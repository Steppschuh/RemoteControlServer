Public Module Authentication

    Private whitelistedIps As List(Of String)

    Public Sub initialize()
        Logger.add("Initializing authenticator")
        whitelistedIps = New List(Of String)
    End Sub

    Public Function isProtected() As Boolean
        If Settings.usePin Or Settings.useWhiteList Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function isAuthenticated(ByVal ip As String) As Boolean
        If Settings.useWhiteList Then
            Return isWhiteListed(ip)
        Else
            Return True
        End If
    End Function

    Public Function isWhiteListed(ByVal ip As String) As Boolean
        If whitelistedIps.Contains(ip) Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function isWhiteListed(ByVal app As App) As Boolean
        Return isWhiteListed(app.ip)
    End Function

    Public Sub addToWhiteList(ByVal ip As String)
        If Not isWhiteListed(ip) Then
            whitelistedIps.Add(ip)
        End If
    End Sub

    Public Sub removeFromWhiteList(ByVal ip As String)
        If isWhiteListed(ip) Then
            whitelistedIps.Remove(ip)
        End If
    End Sub

End Module
