Imports System.Windows.Forms

Module MouseV3

    Private scrollAmount As Integer

    Private currentGesture As Byte = 0
    Private Const GESTURE_NONE As Byte = 0
    Private Const GESTURE_ZOOM As Byte = 1
    Private Const GESTURE_SCROLL As Byte = 2

    Private Const TOLERANCE As Byte = 10

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

    Private mousePadDown, mouseLeftDown, mouseRightDown, isMultitouch As Boolean

    Structure PointAPI
        Public x As Int32
        Public y As Int32
    End Structure

    Public Declare Function GetCursorPos Lib "user32" (ByRef lpPoint As PointAPI) As Boolean

    

#Region "Basic actions"

    Public Sub leftMouseDown()
        mouse_event(MOUSE_LEFT_DOWN, 0, 0, 0, 0)
    End Sub

    Public Sub leftMouseUp()
        mouse_event(MOUSE_LEFT_UP, 0, 0, 0, 0)
    End Sub

    Public Sub leftClick()
        leftMouseDown()
        leftMouseUp()
    End Sub

    Public Sub rightMouseDown()
        mouse_event(MOUSE_RIGHT_DOWN, 0, 0, 0, 0)
    End Sub

    Public Sub rightMouseUp()
        mouse_event(MOUSE_RIGHT_UP, 0, 0, 0, 0)
    End Sub

    Public Sub rightClick()
        rightMouseDown()
        rightMouseUp()
    End Sub

#End Region

#Region "Pointer handling"

    Public Sub parsePointerData(ByVal data As Byte())
        'Pointer data starts at byte index 3
        'Each pointer has one byte for X and Y value

        Dim pointerDataOffset As Byte = 3
        Dim pointerCount As Byte = (data.Length - pointerDataOffset) / 2

        pointers = New List(Of TouchPoint)

        For i As Byte = 0 To pointerCount - 1 Step 1
            Dim currentPointer As New TouchPoint
            currentPointer.x = Converter.byteFromSByte(data(pointerDataOffset + i))
            currentPointer.y = Converter.byteFromSByte(data(pointerDataOffset + i + 1))
            currentPointer.timestamp = Date.Now.Ticks

            pointers.Add(currentPointer)
        Next

        updateCursorPosition()
    End Sub

    Private Sub updateCursorPosition()
        'User last pointer data to modify cursor position
        If Not pointers.Count = 1 Then
            Return
        End If

        cursorPositonCurrent = getCursorPosition()
        P1_New = New Point(pointers(0).x, pointers(0).y)

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

#End Region

#Region "Helper functions"

    Public Function getCursorPosition() As Point
        Dim lpPoint As PointAPI
        GetCursorPos(lpPoint)
        Dim cursorPosition As Point = New Point(lpPoint.x, lpPoint.y)
        Return cursorPosition
    End Function

    Public Function valueMatchesTolerance(ByVal val1 As Decimal, ByVal val2 As Decimal, Optional ByVal tolerance As Integer = TOLERANCE) As Boolean
        If val1 < val2 + tolerance And val1 > val2 - tolerance Then
            Return True
        Else
            Return False
        End If
    End Function

#End Region


End Module
