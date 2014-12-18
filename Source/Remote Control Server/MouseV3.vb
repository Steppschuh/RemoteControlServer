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

    Private pointers As List(Of TouchPoint)

    Public mousePadDown, mouseLeftDown, mouseRightDown, isMultitouch As Boolean

    Structure PointAPI
        Public x As Int32
        Public y As Int32
    End Structure

    Public Declare Function GetCursorPos Lib "user32" (ByRef lpPoint As PointAPI) As Boolean



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
        mousePadDown = True
        P1_Last = P_ORIGIN
        P1_Start = P_ORIGIN

        pointers = New List(Of TouchPoint)
        Dim currentPointer As New TouchPoint()
        currentPointer.x = 0
        currentPointer.y = 0
        currentPointer.timestamp = Date.Now.Ticks
        pointers.Add(currentPointer)
    End Sub

    Public Sub pointersUp()
        mousePadDown = False
        checkForClick()
    End Sub

    Public Sub parsePointerData(ByVal data As Byte())
        'Pointer data starts at byte index 3
        'Each pointer has two bytes for X and two for Y value

        Dim pointerDataOffset As Byte = 3
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

        updateCursorPosition()
    End Sub

    Private Sub updateCursorPosition()
        'User last pointer data to modify cursor position
        If Not pointers.Count = 1 Or Not mousePadDown Then
            Return
        End If

        P1_New = New Point(pointers(0).x, pointers(0).y)

        If P1_Last = P_ORIGIN Then
            P1_Start = P1_New
            P1_Last = P1_New
            pointers(0).timestamp = Date.Now.Ticks
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

        P1_Last = P1_New

        cursorPositonNew = cursorPositonCurrent + P1_Rel
        SetCursorPos(cursorPositonNew.X, cursorPositonNew.Y)
    End Sub

    Private Sub checkForClick()
        For Each pointer As TouchPoint In pointers
            'Down and Up event timestamp in a short timeframe?
            Dim timeDelta As Long = (Date.Now.Ticks - pointer.timestamp) / 1000000

            If timeDelta < CLICK_DELAY_TOLERANCE Then
                'Down and Up event in the same location?
                Dim offsetX As Integer = Math.Sqrt((P1_Start.X - pointer.x) ^ 2)
                Dim offsetY As Integer = Math.Sqrt((P1_Start.Y - pointer.y) ^ 2)

                If offsetX < CLICK_OFFSET_TOLERANCE And offsetY < CLICK_OFFSET_TOLERANCE Then
                    leftClick()
                End If
            Else
                Logger.add("Dif: " & timeDelta)
            End If
        Next
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
