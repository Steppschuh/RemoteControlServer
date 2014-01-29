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

    Public Function isAuthenticated(ByVal ip As String, ByVal pin As String) As Boolean
        If Not isProtected() Then
            Return True
        Else
            If Settings.useWhiteList Then
                If isWhiteListed(ip) Then
                    Return True
                End If
            End If
            If Settings.usePin Then
                If Settings.pin.Equals(pin) Then
                    Return True
                End If
            End If
            Return False
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
