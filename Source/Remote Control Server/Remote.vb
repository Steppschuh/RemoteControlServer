Imports System.Management
Imports System.Runtime.InteropServices

Module Remote

    Public Declare Function SetCursorPos Lib "user32" (ByVal X As Integer, ByVal Y As Integer) As Integer
    Public Declare Sub mouse_event Lib "user32" Alias "mouse_event" (ByVal dwFlags As Integer, ByVal dx As Integer, ByVal dy As Integer, ByVal cButtons As Integer, ByVal dwExtraInfo As Integer)
    Public Declare Unicode Sub keybd_event Lib "user32.dll" (ByVal bVk As Byte, ByVal bScan As Byte, ByVal dwFlags As Int32, ByVal dwExtraInfo As Int32)

    Public latestApi As Byte = 3
    Public lastCommand As Command = New Command

    Public Sub processCommand(ByRef command As Command)
        lastCommand = command

        Select Case command.api
            Case 3
                ApiV3.parseCommand(command)
            Case 2
                ApiV2.parseCommand(command)
            Case 1
                ApiV1.parseCommand(command)
            Case Else
                'Use latest available API
                ApiV3.parseCommand(command)
        End Select

        Server.status = "Active"
        Server.getApp(command.source).status = "Active"
        Server.getApp(command.source).isConnected = True
        Server.getApp(command.source).isActive = False
    End Sub

End Module
