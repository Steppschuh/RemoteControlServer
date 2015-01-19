Imports System.Windows.Forms

Module MouseV3

    Private scrollAmount As Integer

    Private currentGesture As Byte = 0
    Private Const GESTURE_NONE As Byte = 0
    Private Const GESTURE_ZOOM As Byte = 1
    Private Const GESTURE_SCROLL As Byte = 2

    Private Const CLICK_OFFSET_TOLERANCE As Byte = 10 'dip
    Private Const CLICK_DELAY_TOLERANCE As Integer = 500

    Private Const SCROLL_OFFSET_TOLERANCE As Integer = 50

    Private Const MOUSE_LEFT_DOWN = &H2
    Private Const MOUSE_LEFT_UP = &H4
    Private Const MOUSE_RIGHT_DOWN = &H8
    Private Const MOUSE_RIGHT_UP = &H10
    Private Const MOUSE_WHEEL As Int32 = 2048
    Private Const MOUSE_WHEEL_DELTA As Int32 = 120

    Private cursorPositonNew, cursorPositonCurrent, cursorPositonDown As Point

    Private P_ORIGIN As Point = New Point(0, 0)
    Private P1_New, P2_New, P3_New As Point
    Private P1_Rel, P2_Rel, P3_Rel As Point
    Private P1_Start, P2_Start, P3_Start As Point
    Private P1_Last, P2_Last, P3_Last As Point
    Private P3_Vektor_New, P3_Vektor_Last, P3_Vektor_Start, p3_Vektor_Event As Decimal

    Private dateLeftDown As DateTime
    Private dateLeftUp As DateTime

    Private pointDown As TouchPoint
    Private pointers As List(Of TouchPoint)
    Private downEventIndex As Integer = -1
    Private moveEventIndex As Integer = -1

    Public mousePadDown, mouseLeftDown, mouseRightDown, isMultitouch As Boolean

    Structure PointAPI
        Public x As Int32
        Public y As Int32
    End Structure

    Public Declare Function GetCursorPos Lib "user32" (ByRef lpPoint As PointAPI) As Boolean

    Dim pointerLock As New Object


#Region "Basic actions"

    Public Sub leftMouseDown()
        mouseLeftDown = True
        mouse_event(MOUSE_LEFT_DOWN, 0, 0, 0, 0)
    End Sub

    Public Sub leftMouseUp()
        mouseLeftDown = False
        mouse_event(MOUSE_LEFT_UP, 0, 0, 0, 0)
    End Sub

    Public Sub leftClick()
        leftMouseDown()
        leftMouseUp()
    End Sub

    Public Sub rightMouseDown()
        mouseRightDown = True
        mouse_event(MOUSE_RIGHT_DOWN, 0, 0, 0, 0)
    End Sub

    Public Sub rightMouseUp()
        mouseRightDown = False
        mouse_event(MOUSE_RIGHT_UP, 0, 0, 0, 0)
    End Sub

    Public Sub rightClick()
        rightMouseDown()
        rightMouseUp()
    End Sub

#End Region

#Region "Pointer handling"

    Public Sub pointersDown()
        If mousePadDown Then
            Return
        End If

        'Logger.add("pointersDown 1")

        mousePadDown = True
        P1_Start = P_ORIGIN
        P1_Last = P_ORIGIN

        pointers = New List(Of TouchPoint)
        Dim currentPointer As New TouchPoint()
        currentPointer.x = 0
        currentPointer.y = 0
        pointers.Add(currentPointer)

        pointDown = New TouchPoint()
        pointDown.timestamp = Date.Now.Ticks / TimeSpan.TicksPerMillisecond

        'Logger.add("pointersDown 2")
    End Sub

    Public Sub pointersUp()
        'Logger.add("pointersUp 1")

        checkForClick()

        P1_Start = P_ORIGIN
        P1_Last = P_ORIGIN
        P2_Start = P_ORIGIN
        P2_Last = P_ORIGIN
        P3_Start = P_ORIGIN
        P3_Last = P_ORIGIN
        P3_Vektor_Start = 0
        P3_Vektor_Last = 0

        mousePadDown = False
        currentGesture = GESTURE_NONE
        pointers = New List(Of TouchPoint)

        'Logger.add("pointersUp 2")
    End Sub

    Public Sub parsePointerData(ByVal data As Byte())
        Try
            'Pointer data starts at byte index 3
            'Each pointer has two bytes for X and two for Y value

            Dim newDownEventIndex As Integer = data(3)
            newDownEventIndex = (newDownEventIndex << 8) + data(4)

            Dim newMoveEventIndex As Integer = data(5)
            newMoveEventIndex = (newMoveEventIndex << 8) + data(6)

            'Check if command is old and should be ignored
            If newDownEventIndex < downEventIndex Then
                'Logger.add("Ignoring old command (down index)")
                'Return
            ElseIf newDownEventIndex < downEventIndex And newMoveEventIndex < moveEventIndex Then
                'Logger.add("Ignoring old command (move index)")
                'Return
            End If

            Dim pointerDataOffset As Byte = 7
            Dim pointerLength As Byte = 4
            Dim pointerCount As Byte = (data.Length - pointerDataOffset) / pointerLength

            pointers = New List(Of TouchPoint)

            For i As Byte = 0 To pointerCount - 1 Step 1
                Dim currentPointer As New TouchPoint

                Dim x As Integer = data(pointerDataOffset + (i * pointerLength))
                x = (x << 8) + data(pointerDataOffset + (i * pointerLength) + 1)

                Dim y As Integer = data(pointerDataOffset + (i * pointerLength) + 2)
                y = (y << 8) + data(pointerDataOffset + (i * pointerLength) + 3)

                currentPointer.x = x
                currentPointer.y = y

                pointers.Add(currentPointer)
            Next

            'Logger.add("Down #" & newDownEventIndex & " Move #" & newMoveEventIndex & " " & pointers(0).x & "|" & pointers(0).y)

            If newDownEventIndex > downEventIndex Then
                'New cursor update should be processed after pointerUp and pointerDown event
                moveEventIndex = -1
                pointersUp()
            End If

            downEventIndex = newDownEventIndex
            moveEventIndex = newMoveEventIndex

            If pointers.Count = 1 Then
                updateCursorPosition()
            ElseIf pointers.Count > 1 Then
                parseMultitouch()
            End If

        Catch ex As Exception
        End Try
    End Sub

    Public Sub parseAbsolutePointerData(ByVal data As Byte(), Optional ByVal isPresenter As Boolean = False)
        'Pointer data starts at byte index 3
        'Each pointer has two bytes for X and two for Y value

        Dim absoluteMaximum As Integer = 32767
        Dim pointerDataOffset As Byte = 3
        Dim pointerCount As Byte = (data.Length - pointerDataOffset) / 4

        pointers = New List(Of TouchPoint)

        Dim currentPointer As New TouchPoint

        Dim x As Integer = data(pointerDataOffset)
        x = (x << 8) + data(pointerDataOffset + 1)

        Dim y As Integer = data(pointerDataOffset + 2)
        y = (y << 8) + data(pointerDataOffset + 3)

        Dim locations As System.Drawing.Point() = getScreenBounds(Screenshot.screenIndex)
        Dim startLocation As System.Drawing.Point = locations(0)
        Dim endLocation As System.Drawing.Point = locations(1)
        Dim screenSize As Size = New Size(endLocation.X - startLocation.X, endLocation.Y - startLocation.Y)

        currentPointer.x = (x * screenSize.Width) / absoluteMaximum
        currentPointer.y = (y * screenSize.Height) / absoluteMaximum

        cursorPositonNew.X = currentPointer.x
        cursorPositonNew.Y = currentPointer.y

        'Add absolute pointer position to pointer list to enable click detection
        pointers.Add(currentPointer)

        P1_Start.X = currentPointer.x
        P1_Start.Y = currentPointer.y

        SetCursorPos(cursorPositonNew.X, cursorPositonNew.Y)

        If isPresenter Then
            updatePointerPosition()
        Else

        End If
    End Sub

    Private Sub updatePointerPosition()
        If pointers.Count > 0 Then
            If Not Server.pointer.isVisible Then
                Server.pointer.showPointer()
            End If
            Dim locations As System.Drawing.Point() = Screenshot.getScreenBounds(Screenshot.screenIndex)

            Dim startLocation As System.Drawing.Point = locations(0)
            Dim endLocation As System.Drawing.Point = locations(1)
            Dim screenSize As Size = New Size(endLocation.X - startLocation.X, endLocation.Y - startLocation.Y)

            Dim width As Integer = screenSize.Width
            Dim height As Integer = screenSize.Height

            Dim pointerPosition As New Point
            Dim pointerOffset As Integer = 50
            pointerPosition.X = Math.Max(0, pointers(0).x - pointerOffset)
            pointerPosition.X = Math.Min(width - 100, pointerPosition.X)
            pointerPosition.Y = Math.Max(0, pointers(0).y - pointerOffset)
            pointerPosition.Y = Math.Min(height - 100, pointerPosition.Y)

            Server.pointer.setPointerPosition(pointerPosition)
            Server.pointer.fadeOutPointer()
        End If
    End Sub

    Private Sub updateCursorPosition()
        If mousePadDown = False Then
            'onMove event before onDown event received
            'Logger.add("onMove event before onDown event received")
            pointersDown()
            Return
        End If

        'User last pointer data to modify cursor position
        P1_New = New Point(pointers(0).x, pointers(0).y)

        If P1_Start = P_ORIGIN Or P1_Start = P_ORIGIN Then
            'Logger.add("P_Start & P_Last set to P_New: " & P1_New.X & "|" & P1_New.Y)
            P1_Start = P1_New
            P1_Last = P1_New
            Return
        End If

        cursorPositonCurrent = getCursorPosition()

        If Settings.mouseAcceleration = 1 Then
            'No acceleration
            P1_Rel.X = (P1_New.X - P1_Last.X) * Settings.mouseSensitivity
            P1_Rel.Y = (P1_New.Y - P1_Last.Y) * Settings.mouseSensitivity
        Else
            'Acceleration
            P1_Rel.X = (P1_New.X - P1_Last.X)
            P1_Rel.Y = (P1_New.Y - P1_Last.Y)

            If Not (P1_Rel.X = 0 And P1_Rel.Y = 0) Then
                If P1_Rel.X > 0 Then
                    P1_Rel.X = (P1_Rel.X ^ Settings.mouseAcceleration) * Settings.mouseSensitivity
                Else
                    P1_Rel.X = P1_Rel.X * (-1)
                    P1_Rel.X = (P1_Rel.X ^ Settings.mouseAcceleration)
                    P1_Rel.X = P1_Rel.X * Settings.mouseSensitivity
                    P1_Rel.X = P1_Rel.X * (-1)
                End If

                If P1_Rel.Y > 0 Then
                    P1_Rel.Y = (P1_Rel.Y ^ Settings.mouseAcceleration) * Settings.mouseSensitivity
                Else
                    P1_Rel.Y = P1_Rel.Y * (-1)
                    P1_Rel.Y = (P1_Rel.Y ^ Settings.mouseAcceleration)
                    P1_Rel.Y = P1_Rel.Y * Settings.mouseSensitivity
                    P1_Rel.Y = P1_Rel.Y * (-1)
                End If
            End If
        End If

        If P1_Rel.X < -700 Or P1_Rel.X > 700 Or P1_Rel.Y < -700 Or P1_Rel.Y > 700 Then
            'Logger.add("Rel: " & P1_Rel.X & "|" & P1_Rel.Y & " New: " & P1_New.X & "|" & P1_New.Y & " Old: " & P1_Last.X & "|" & P1_Last.Y)
            pointersUp()
            Return
        End If

        P1_Last = P1_New

        'Logger.add("Rel: " & P1_Rel.X & "|" & P1_Rel.Y)

        cursorPositonNew = cursorPositonCurrent + P1_Rel
        SetCursorPos(cursorPositonNew.X, cursorPositonNew.Y)
    End Sub

    Private Sub parseMultitouch()
        If mousePadDown = False Then
            'onMove event before onDown event received
            'Logger.add("onMove event before onDown event received")
            pointersDown()
            Return
        End If

        'User last pointer data to modify cursor position
        P1_New = New Point(pointers(0).x, pointers(0).y)
        P2_New = New Point(pointers(1).x, pointers(1).y)

        If P1_Start = P_ORIGIN Or P1_Last = P_ORIGIN Or P2_Start = P_ORIGIN Or P2_Last = P_ORIGIN Then
            'Logger.add("P_Start & P_Last set to P_New: " & P1_New.X & "|" & P1_New.Y)
            P1_Start = P1_New
            P1_Last = P1_New
            P2_Start = P2_New
            P2_Last = P2_New
            Return
        End If

        P1_Rel.X = (P1_New.X - P1_Last.X) * Settings.mouseSensitivity
        P1_Rel.Y = (P1_New.Y - P1_Last.Y) * Settings.mouseSensitivity

        If P1_Rel.X < -700 Or P1_Rel.X > 700 Or P1_Rel.Y < -700 Or P1_Rel.Y > 700 Then
            'Logger.add("Rel: " & P1_Rel.X & "|" & P1_Rel.Y & " New: " & P1_New.X & "|" & P1_New.Y & " Old: " & P1_Last.X & "|" & P1_Last.Y)
            pointersUp()
            Return
        End If

        P1_Last = P1_New
        P2_Last = P2_New

        processMultitouch()
    End Sub

    Public Sub processMultitouch()
        P3_New.X = Math.Round((P1_New.X + P2_New.X) / 2)
        P3_New.Y = Math.Round((P1_New.Y + P2_New.Y) / 2)
        P3_Vektor_New = Converter.getPointDistance(P1_New, P2_New)


        If P3_Start = P_ORIGIN Then
            P3_Start = P3_New
        End If

        If P3_Vektor_Start = 0 Then
            P3_Vektor_Start = P3_Vektor_New
        End If

        If currentGesture = GESTURE_NONE Then
            'Logger.add("P1: " & P1_New.X & "|" & P1_New.Y & " " & "P2: " & P2_New.X & "|" & P2_New.Y & " " & "P3: " & P3_New.X & "|" & P3_New.Y)
            'Logger.add("Determining gesture. Distance: " & P3_Vektor_New & " X: " & P3_New.X - P3_Start.X & " Y: " & P3_New.Y - P3_Start.Y)

            If valueMatchesTolerance(P3_Vektor_New, P3_Vektor_Start, SCROLL_OFFSET_TOLERANCE) Then
                'distance constant
                If valueMatchesTolerance(P3_New.X, P3_Start.X, SCROLL_OFFSET_TOLERANCE) Then
                    'horizontal constant
                    If valueMatchesTolerance(P3_New.Y, P3_Start.Y, SCROLL_OFFSET_TOLERANCE / 2) Then
                        'vertical constant
                    Else
                        'scroll
                        'Logger.add("Gesture set to scroll")
                        currentGesture = GESTURE_SCROLL
                    End If
                End If
            Else
                'zoom
                'Logger.add("Gesture set to zoom")
                currentGesture = GESTURE_ZOOM
                p3_Vektor_Event = P3_Vektor_New + 20
            End If
        End If

        If currentGesture = GESTURE_ZOOM Then
            'zoom
            If Not valueMatchesTolerance(P3_Vektor_New, p3_Vektor_Event, 25) Then
                scrollAmount = (P3_Vektor_New - P3_Vektor_Last) * (P3_Vektor_New - P3_Vektor_Last)
                If P3_Vektor_New > P3_Vektor_Last Then
                    zoom(1, 1)
                Else
                    'zoom out
                    zoom(-1, 1)
                End If
                p3_Vektor_Event = P3_Vektor_New
            End If

        ElseIf currentGesture = GESTURE_SCROLL Then
            'scroll
            scrollAmount = (P3_New.Y - P3_Last.Y) * (P3_New.Y - P3_Last.Y)
            If P3_New.Y > P3_Last.Y Then
                'scroll down
                scrollRepeat(-30 - scrollAmount, 1)
            ElseIf P3_New.Y < P3_Last.Y Then
                'scroll up
                scrollRepeat(30 + scrollAmount, 1)
            End If
        End If

        P3_Last = P3_New
        P3_Vektor_Last = P3_Vektor_New
    End Sub

    Private Sub checkForClick()
        Try
            'Down and Up event timestamp in a short timeframe?
            Dim timeDelta As Long = ((Date.Now.Ticks / TimeSpan.TicksPerMillisecond) - pointDown.timestamp)

            If timeDelta < CLICK_DELAY_TOLERANCE Then
                Logger.add("Click delay: " & timeDelta)
                'Down and Up event in the same location?
                Dim offsetX As Integer = Math.Sqrt((P1_Start.X - pointers(0).x) ^ 2)
                Dim offsetY As Integer = Math.Sqrt((P1_Start.Y - pointers(0).y) ^ 2)

                If offsetX < CLICK_OFFSET_TOLERANCE And offsetY < CLICK_OFFSET_TOLERANCE Then
                    leftClick()
                End If
            Else
                'Logger.add("Delta: " & timeDelta)
            End If
        Catch ex As Exception
        End Try
    End Sub

#End Region

#Region "Helper functions"

    Public Function getCursorPosition() As Point
        Dim lpPoint As PointAPI
        GetCursorPos(lpPoint)
        Dim cursorPosition As Point = New Point(lpPoint.x, lpPoint.y)
        Return cursorPosition
    End Function

    Public Function valueMatchesTolerance(ByVal val1 As Decimal, ByVal val2 As Decimal, Optional ByVal tolerance As Integer = CLICK_OFFSET_TOLERANCE) As Boolean
        If val1 < val2 + tolerance And val1 > val2 - tolerance Then
            Return True
        Else
            Return False
        End If
    End Function

#End Region


End Module
