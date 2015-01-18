Imports System.Windows.Forms

Module MouseV3

    Private scrollAmount As Integer

    Private currentGesture As Byte = 0
    Private Const GESTURE_NONE As Byte = 0
    Private Const GESTURE_ZOOM As Byte = 1
    Private Const GESTURE_SCROLL As Byte = 2

    Private Const CLICK_OFFSET_TOLERANCE As Byte = 10 'dip
    Private Const CLICK_DELAY_TOLERANCE As Integer = 500

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

        Logger.add("pointersDown 1")

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
        Logger.add("pointersUp 1")

        checkForClick()

        P1_Start = P_ORIGIN
        P1_Last = P_ORIGIN


        mousePadDown = False
        pointers = New List(Of TouchPoint)
        P1_Start = P_ORIGIN
        P1_Last = P_ORIGIN

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
            Dim pointerCount As Byte = (data.Length - pointerDataOffset) / 4

            pointers = New List(Of TouchPoint)

            For i As Byte = 0 To pointerCount - 1 Step 1
                Dim currentPointer As New TouchPoint

                Dim x As Integer = data(pointerDataOffset + i)
                x = (x << 8) + data(pointerDataOffset + i + 1)

                Dim y As Integer = data(pointerDataOffset + i + 2)
                y = (y << 8) + data(pointerDataOffset + i + 3)

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

            updateCursorPosition()
        Catch ex As Exception
        End Try
    End Sub

    Public Sub parseAbsolutePointerData(ByVal data As Byte())
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

        SetCursorPos(cursorPositonNew.X, cursorPositonNew.Y)

        'Add absolute pointer position to pointer list to enable click detection
        pointers.Add(currentPointer)

        P1_Start.X = currentPointer.x
        P1_Start.Y = currentPointer.y
    End Sub

    Private Sub updateCursorPosition()
        If Not pointers.Count = 1 Then
            'No pointer data available or multitouch
            Return
        End If

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
