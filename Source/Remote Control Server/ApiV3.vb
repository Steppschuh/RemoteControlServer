Module ApiV3

    'API v3 uses commands similar to API v2 but supports communication from the server to an app
    'Just a placeholder for now

    Public Const COMMAND_IDENTIFIER As Byte = 129

    Public Function isBroadcast(ByVal command As Command) As Boolean
        Return False
    End Function

    Public Sub requestPin(ByRef app As App)

    End Sub

    Public Sub answerBroadCast(ByRef app As App)

    End Sub

    Public Sub refuseBroadCast(ByRef app As App)

    End Sub

    Public Sub parseCommand(ByRef command As Command)

    End Sub

End Module
