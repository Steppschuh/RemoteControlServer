Imports System.Windows.Forms

Module MouseV2

    Public Const cmd_mouse_down As Byte = 0
    Public Const cmd_mouse_up As Byte = 1
    Public Const cmd_mouse_down_1 As Byte = 2
    Public Const cmd_mouse_up_1 As Byte = 3
    Public Const cmd_mouse_move As Byte = 4
    Public Const cmd_mouse_left_down As Byte = 5
    Public Const cmd_mouse_left_up As Byte = 6
    Public Const cmd_mouse_right_down As Byte = 7
    Public Const cmd_mouse_right_up As Byte = 8
    Public Const cmd_mouse_scroll As Byte = 9
    Public Const cmd_mouse_set As Byte = 10
    Public Const cmd_mouse_left_click As Byte = 11
    Public Const cmd_mouse_right_click As Byte = 12

    Private scrollAmount As Integer

    Private currentGesture As Byte = 0
    Private Const GESTURE_NONE As Byte = 0
    Private Const GESTURE_ZOOM As Byte = 1
    Private Const GESTURE_SCROLL As Byte = 2

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

    Private P1_Down As DateTime = DateTime.Now
    Private P1_Up As DateTime = DateTime.Now
    Private P2_Down As DateTime = DateTime.Now
    Private P2_Up As DateTime = DateTime.Now
    Private P3_Down As DateTime = DateTime.Now
    Private P3_Up As DateTime = DateTime.Now

    Private mousePadDown, mouseLeftDown, mouseRightDown, isMultitouch As Boolean

    'Laser pointer
    Public Const cmd_pointer_start As Byte = 0
    Public Const cmd_pointer_end As Byte = 1
    Public Const cmd_pointer_move As Byte = 2
    Public Const cmd_pointer_calibrate As Byte = 3

    Public X_New, Y_New, Z_New As Single
    Public X_Rel, Y_Rel, Z_Rel As Single
    Public X_Def, Y_Def, Z_Def As Single

    Structure PointAPI
        Public x As Int32
        Public y As Int32
    End Structure

    Public Declare Function GetCursorPos Lib "user32" (ByRef lpPoint As PointAPI) As Boolean

    Public Function getCursorPosition() As Point
        Dim lpPoint As PointAPI
        GetCursorPos(lpPoint)
        Dim cursorPosition As Point = New Point(lpPoint.x, lpPoint.y)
        Return cursorPosition
    End Function

    Public Function valueMatchesTolerance(ByVal val1 As Decimal, ByVal val2 As Decimal, Optional ByVal tolerance As Integer = 20) As Boolean
        If val1 < val2 + tolerance And val1 > val2 - tolerance Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function getPointAt(ByVal data As Byte(), ByVal index As Integer) As Point
        Dim P As Point
        Try
            If index = 0 Then
                P.X = data(3)
                P.Y = data(4)
            ElseIf index = 1 Then
                P.X = data(5)
                P.Y = data(6)
            End If
        Catch ex As Exception
        End Try
        Return P
    End Function

    Public Function commandGetPoint(ByVal messageBytes As Byte(), ByVal ID As Integer) As Point
        Dim P As Point
        Try
            If ID = 0 Then
                P.X = messageBytes(3)
                P.Y = messageBytes(4)
            ElseIf ID = 1 Then
                P.X = messageBytes(5)
                P.Y = messageBytes(6)
            End If
        Catch ex As Exception
            'log(ex.ToString)
        End Try
        Return P
    End Function

    Public Function isPointerDown(ByVal pointer As Point) As Boolean
        If pointer = P_ORIGIN Then
            Return False
        Else
            Return True
        End If
    End Function

    Public Sub moveCursor()
        cursorPositonCurrent = getCursorPosition()
        If isPointerDown(P2_New) = True Then
            '2 Pointer down
            If mouseLeftDown = True Then
                'Left mouse down and moving
                P2_Rel.X = (P2_New.X - P2_Last.X) * Settings.mouseSensitivity
                P2_Rel.Y = (P2_New.Y - P2_Last.Y) * Settings.mouseSensitivity
                cursorPositonNew = cursorPositonCurrent + P2_Rel
                P2_Last = P2_New
                SetCursorPos(cursorPositonNew.X, cursorPositonNew.Y)
            ElseIf mouseRightDown = True Then
                'Right mouse down and moving
            End If
        Else
            '1 Pointer down
            If P1_Last = P_ORIGIN Then
                P1_Last = P1_New
            Else
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

                cursorPositonNew = cursorPositonCurrent + P1_Rel
                P1_Last = P1_New
                SetCursorPos(cursorPositonNew.X, cursorPositonNew.Y)
            End If
        End If
    End Sub

    Public Sub scrollRepeat(ByVal value As Integer, Optional ByVal count As Integer = 1)
        For i = 0 To count
            mouse_event(MOUSE_WHEEL, 0, 0, value, 0)
            i += 1
        Next
    End Sub

    Public Sub leftClickRepeat(ByVal count As Integer)
        For i = 1 To count
            mouse_event(MOUSE_LEFT_DOWN, 0, 0, 0, 0)
            mouse_event(MOUSE_LEFT_UP, 0, 0, 0, 0)
            i += 1
        Next
    End Sub

    Public Sub zoom(ByVal value As Integer, Optional ByVal count As Integer = 1)
        Keyboard.sendKeyDown(Keys.LControlKey)
        scrollRepeat(value, count)
        Keyboard.sendKeyUp(Keys.LControlKey)
    End Sub

    Public Sub processMultitouch()
        P3_New.X = Math.Round((P1_New.X + P2_New.X) / 2)
        P3_New.Y = Math.Round((P1_New.Y + P2_New.Y) / 2)
        P3_Vektor_New = Converter.getPointDistance(P1_New, P2_New)

        If currentGesture = GESTURE_NONE Then
            If valueMatchesTolerance(P3_Vektor_New, P3_Vektor_Start, 40) Then
                'distance constant
                If valueMatchesTolerance(P3_New.X, P3_Start.X, 40) Then
                    'horizontal constant
                    If valueMatchesTolerance(P3_New.Y, P3_Start.Y, 40) Then
                        'vertical constant
                    Else
                        'scroll
                        currentGesture = GESTURE_SCROLL
                    End If
                End If
            Else
                'zoom
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

    Public Sub parseCursorMove(ByVal messageBytes As Byte())
        P1_New = commandGetPoint(messageBytes, 0)
        If isMultitouch = True Then
            P2_New = commandGetPoint(messageBytes, 1)
            processMultitouch()
        Else
            P2_New = P_ORIGIN
            moveCursor()
        End If
    End Sub

    Public Sub parseCursorSet(ByVal messageBytes As Byte())
        Dim x As Integer = 0
        Dim y As Integer = 0
        Try
            x = messageBytes(3) * 255 + messageBytes(4)
            y = messageBytes(5) * 255 + messageBytes(6)

            Dim locations As System.Drawing.Point() = Screenshot.getScreenBounds(Screenshot.screenIndex)
            x += locations(0).X
            y += locations(0).Y

            SetCursorPos(x, y)
        Catch ex As Exception
        End Try
    End Sub

    Public Sub parseScroll(ByVal messageBytes As Byte())
        P1_New = commandGetPoint(messageBytes, 0)
        scrollAmount = (P1_New.Y - P1_Last.Y) * 20
        If P1_New.Y > P1_Last.Y Then
            'scroll down
            scrollRepeat(0 - scrollAmount, 1)
        ElseIf P1_New.Y < P1_Last.Y Then
            'scroll up
            scrollRepeat(0 - scrollAmount, 1)
        End If
        P1_Last = P1_New
    End Sub

    Public Sub parseClick(ByVal messageBytes As Byte())
        Select Case messageBytes(2)
            Case cmd_mouse_down
                '1 Pointer Down
                isMultitouch = False
                P1_Start = commandGetPoint(messageBytes, 0)
                P1_Last = P1_Start
                cursorPositonDown = getCursorPosition()
                mousePadDown = True
                P1_Down = DateTime.Now
            Case cmd_mouse_up
                '0 Pointer Down
                mousePadDown = False
                isMultitouch = False

                P1_Last = P_ORIGIN
                P2_Last = P_ORIGIN
                P1_New = P_ORIGIN
                P1_Up = DateTime.Now

                'Check if it was a click
                If P1_Up.Subtract(P1_Down).TotalMilliseconds < 150 Then
                    If -10 < P1_Start.X - P1_Last.X < 10 And -10 < P1_Start.Y - P1_Last.Y < 10 Then
                        SetCursorPos(cursorPositonDown.X, cursorPositonDown.Y)
                        leftClickRepeat(1)
                    End If
                End If

            Case cmd_mouse_down_1
                '2 Pointer Down
                isMultitouch = True
                P1_Start = commandGetPoint(messageBytes, 0)
                P1_New = P1_Start
                P1_Last = P1_Start
                P1_Down = DateTime.Now

                P2_Start = commandGetPoint(messageBytes, 1)
                P2_New = P2_Start
                P2_Last = P2_Start
                P2_Down = DateTime.Now
                currentGesture = GESTURE_NONE

                P3_Start.X = Math.Round((P1_New.X + P2_New.X) / 2)
                P3_Start.Y = Math.Round((P1_New.Y + P2_New.Y) / 2)
                P3_New = P3_Start
                P3_Last = P3_Start
                P3_Vektor_Start = Converter.getPointDistance(P1_New, P2_New)
            Case cmd_mouse_up_1
                '1 Pointer Down
                P2_Up = DateTime.Now
                isMultitouch = False
                P1_Last = P_ORIGIN
                P2_New = P_ORIGIN
                P3_New = P_ORIGIN
                P3_Last = P_ORIGIN
                P3_Start = P_ORIGIN
            Case cmd_mouse_left_down
                mouse_event(MOUSE_LEFT_DOWN, 0, 0, 0, 0)
                dateLeftDown = DateTime.Now
                mouseLeftDown = True
            Case cmd_mouse_left_up
                mouse_event(MOUSE_LEFT_UP, 0, 0, 0, 0)
                dateLeftUp = DateTime.Now
                mouseLeftDown = False
            Case cmd_mouse_right_down
                mouseRightDown = True
                mouse_event(MOUSE_RIGHT_DOWN, 0, 0, 0, 0)
            Case cmd_mouse_right_up
                mouseRightDown = False
                mouse_event(MOUSE_RIGHT_UP, 0, 0, 0, 0)
            Case cmd_mouse_left_click
                mouse_event(MOUSE_LEFT_DOWN, 0, 0, 0, 0)
                mouse_event(MOUSE_LEFT_UP, 0, 0, 0, 0)
            Case cmd_mouse_right_click
                mouse_event(MOUSE_RIGHT_DOWN, 0, 0, 0, 0)
                mouse_event(MOUSE_RIGHT_UP, 0, 0, 0, 0)
        End Select
    End Sub

    Public Sub parsePointer(ByVal messageBytes As Byte())
        Try
            X_New = messageBytes(3)
            Y_New = messageBytes(4)
            Z_New = 0

            If X_New > 99 Then
                X_New = 100 - X_New
            End If

            If Y_New > 99 Then
                Y_New = 100 - Y_New
            End If

            X_New = X_New - X_Def
            Y_New = Y_New - Y_Def

            'log(X_New & " | " & Y_New)


            X_Rel = ((Y_New * (-1)) * Settings.motionAcceleration * 20) / 100
            Y_Rel = ((X_New * (1)) * Settings.motionAcceleration * 20) / 100

            If X_Rel > Settings.motionFilter Or X_Rel < Settings.motionFilter * -1 Then
            Else
                X_Rel = 0
            End If
            If Y_Rel > Settings.motionFilter Or Y_Rel < Settings.motionFilter * -1 Then
            Else
                Y_Rel = 0
            End If
        Catch ex As Exception
            Logger.add(ex.ToString)
        End Try

        'Pointer
        If Not X_Rel = 0 And Not Y_Rel = 0 Then
            cursorPositonCurrent = Server.pointer.getPointerPosition()
            If Y_Rel <> 0 Then
                If Y_Rel < 0 Then
                    cursorPositonNew.X = cursorPositonCurrent.X - Y_Rel * Y_Rel
                ElseIf Y_Rel > 0 Then
                    cursorPositonNew.X = cursorPositonCurrent.X + Y_Rel * Y_Rel
                End If
            Else
                cursorPositonNew.X = cursorPositonCurrent.X
            End If
            If X_Rel <> 0 Then
                If X_Rel < 0 Then
                    cursorPositonNew.Y = cursorPositonCurrent.Y - X_Rel * X_Rel
                ElseIf X_Rel > 0 Then
                    cursorPositonNew.Y = cursorPositonCurrent.Y + X_Rel * X_Rel
                End If
            Else
                cursorPositonNew.Y = cursorPositonCurrent.Y
            End If

            If cursorPositonNew.X > Screen.PrimaryScreen.Bounds.Width - 50 Then
                cursorPositonNew.X = Screen.PrimaryScreen.Bounds.Width - 50
            ElseIf cursorPositonNew.X < 0 Then
                cursorPositonNew.X = 0
            End If
            If cursorPositonNew.Y > Screen.PrimaryScreen.Bounds.Height - 50 Then
                cursorPositonNew.Y = Screen.PrimaryScreen.Bounds.Height - 50
            ElseIf cursorPositonNew.Y < 0 Then
                cursorPositonNew.Y = 0
            End If

            Server.pointer.setPointerPosition(New Point(cursorPositonNew.X, cursorPositonNew.Y))
        End If
        Exit Sub
    End Sub

    Public Sub calibratePointer(ByVal messageBytes As Byte())
        Try
            X_Def = messageBytes(3)
            Y_Def = messageBytes(4)

            If X_Def > 99 Then
                X_Def = 100 - X_Def
            End If

            If Y_Def > 99 Then
                Y_Def = 100 - Y_Def
            End If

            Z_Def = 0
        Catch ex As Exception
            X_Def = 0
            Y_Def = 0
            Z_Def = 0
        End Try
    End Sub

    Public Sub parseLaser(ByVal messageBytes As Byte())
        Dim point_org As Point
        Dim point As Point
        point_org = getPointAt(messageBytes, 0)

        Server.pointer.showPointer()

        'log("Laser %: " & point_org.X & " | " & point_org.Y)

        Dim locations As System.Drawing.Point() = Screenshot.getScreenBounds(Screenshot.screenIndex)

        Dim startLocation As System.Drawing.Point = locations(0)
        Dim endLocation As System.Drawing.Point = locations(1)
        Dim screenSize As Size = New Size(endLocation.X - startLocation.X, endLocation.Y - startLocation.Y)

        Dim width As Integer = screenSize.Width
        Dim height As Integer = screenSize.Height

        'log("Screen: " & width & " | " & height)

        point.X = (point_org.X * width) / 255
        point.Y = (point_org.Y * height) / 255

        point.X += startLocation.X
        point.Y += startLocation.Y

        'log("Laser: " & point.X & " | " & point.Y)

        Dim x, y As Integer
        If point.X > startLocation.X + 25 Then
            x = point.X - 25
        Else
            x = startLocation.X
        End If

        If point.Y > startLocation.Y + 25 Then
            y = point.Y - 25
        Else
            x = startLocation.Y
        End If

        Server.pointer.setPointerPosition(New Point(x, y))
        Server.pointer.fadeOutPointer()
    End Sub


#Region "Legacy support"

    'These methods are supporting very old apps, do not use!

    Public Sub parseScroll(ByVal message As String)
        Dim Action As String = ApiV1.getCommandvalue(message)
        If Action.Contains("left_down") Or Action.Contains("zoomout") Then
            'Zoom out
            Keyboard.sendKeyDown(Keys.LControlKey)
            scrollRepeat(-10, 1)
            Keyboard.sendKeyUp(Keys.LControlKey)
        ElseIf Action.Contains("right_down") Or Action.Contains("zoomin") Then
            'Zoom in
            Keyboard.sendKeyDown(Keys.LControlKey)
            scrollRepeat(10, 1)
            Keyboard.sendKeyUp(Keys.LControlKey)
        ElseIf Action.Contains("left2_down") Or Action.Contains("back") Then
            'Back
            Keyboard.sendKeyPress(Keys.BrowserBack)
        ElseIf Action.Contains("right2_down") Or Action.Contains("forward") Then
            'Forward
            Keyboard.sendKeyPress(Keys.BrowserForward)
        ElseIf Action.Contains("pagedown") Then
            Keyboard.sendKeyPress(Keys.PageDown)
        ElseIf Action.Contains("pageup") Then
            Keyboard.sendKeyPress(Keys.PageUp)
        ElseIf Action.Contains("cancel") Then
            Keyboard.sendKeyPress(Keys.BrowserStop)
        ElseIf Action.Contains("refresh") Then
            Keyboard.sendKeyPress(Keys.BrowserRefresh)
        ElseIf Action.Contains("fullscreen") Then
            Keyboard.sendKeyPress(Keys.F11)
        ElseIf Action.Contains("fullexit") Then
            Keyboard.sendKeyPress(Keys.Escape)
        End If
    End Sub

    'Parses the x and y values from a command at an index
    Private Function parsePointAt(ByVal message As String, ByVal index As Integer) As Point
        Dim point As Point
        Dim temp As String 'ACTION_MOVE[#0=82,292;#1=157,116]

        If index = 0 Then
            message = message.Substring(InStr(message, "=")) '82,292;#1=157,116]
            If message.Contains(";") Then
                temp = message.Remove(InStr(message, ";") - 1) '82,292
            Else
                temp = message.Remove(InStr(message, "]") - 1) '82,292
            End If

            point.X = temp.Remove(InStr(temp, ",") - 1) '82
            point.Y = temp.Substring(InStr(temp, ",")) '292
        Else
            If message.Contains(";") Then
                message = message.Substring(InStrRev(message, "=")) '157,116]
                temp = message.Remove(InStr(message, "]") - 1) '157,116
                point.X = temp.Remove(InStr(temp, ",") - 1) '157
                point.Y = temp.Substring(InStr(temp, ",")) '116
            Else
                point.X = -1
                point.Y = -1
            End If
        End If

        Return point
    End Function

    'Parses a legacy mouse remote command
    Public Sub parseMouse(ByVal message As String)
        Dim Action As String = message
        If Action.Contains("ACTION_MOVE") Then
            P1_New = parsePointAt(Action, 0)
            If isMultitouch = True Then
                P2_New = parsePointAt(Action, 1)
                processMultitouch()
            Else
                P2_New = P_ORIGIN
                moveCursor()
            End If
            Exit Sub
        ElseIf Action.Contains("ACTION_DOWN") Then
            If Action.Contains("ACTION_DOWN[") Then
                '1 Pointer Down
                isMultitouch = False
                P1_Start = parsePointAt(Action, 0)
                P1_Last = P1_Start

                cursorPositonDown = getCursorPosition()
                mousePadDown = True
                P1_Down = DateTime.Now
            Else
                '2 Pointer Down
                isMultitouch = True
                P1_Start = parsePointAt(Action, 0)
                P1_New = P1_Start
                P1_Last = P1_Start
                P1_Down = DateTime.Now

                P2_Start = parsePointAt(Action, 1)
                P2_New = P2_Start
                P2_Last = P2_Start
                P2_Down = DateTime.Now
                currentGesture = 0

                P3_Start.X = Math.Round((P1_New.X + P2_New.X) / 2)
                P3_Start.Y = Math.Round((P1_New.Y + P2_New.Y) / 2)
                P3_New = P3_Start
                P3_Last = P3_Start
                P3_Vektor_Start = Converter.getPointDistance(P1_New, P2_New)
            End If
        ElseIf Action.Contains("ACTION_UP") Then
            If Action.Contains("ACTION_UP[") Then
                '0 Pointer Down
                mousePadDown = False
                P1_New = P_ORIGIN
                P1_Up = DateTime.Now

                If P1_Up.Subtract(P1_Down).TotalMilliseconds < 150 Then
                    If -10 < P1_Start.X - P1_Last.X < 10 And -10 < P1_Start.Y - P1_Last.Y < 10 Then
                        SetCursorPos(cursorPositonDown.X, cursorPositonDown.Y)
                        leftClickRepeat(1)
                    End If
                End If
            Else
                '1 Pointer Down
                P2_Up = DateTime.Now
                isMultitouch = False
                P1_Last = P_ORIGIN
                P2_New = P_ORIGIN
                P3_New = P_ORIGIN
                P3_Last = P_ORIGIN
                P3_Start = P_ORIGIN
            End If
            currentGesture = 0
        ElseIf Action.Contains("left_down") Then
            mouse_event(MOUSE_LEFT_DOWN, 0, 0, 0, 0)
            dateLeftDown = DateTime.Now
            mouseLeftDown = True
        ElseIf Action.Contains("left_up") Then
            mouse_event(MOUSE_LEFT_UP, 0, 0, 0, 0)
            dateLeftUp = DateTime.Now
            mouseLeftDown = False
        ElseIf Action.Contains("right_down") Then
            mouseRightDown = True
            mouse_event(MOUSE_RIGHT_DOWN, 0, 0, 0, 0)
        ElseIf Action.Contains("right_up") Then
            mouseRightDown = False
            mouse_event(MOUSE_RIGHT_UP, 0, 0, 0, 0)
        End If
    End Sub

#End Region


End Module
