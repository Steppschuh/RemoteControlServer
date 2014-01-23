Imports System.Windows.Threading

Public Class PointerWindow

    Private Delegate Sub invalidateDelegate()
    Private Delegate Sub setPointerPositionDelegate(ByVal point As Point)

    Private isFadingOut As Boolean = False
    Private isFadingOutTimed As Boolean = False

    Private fadeOutDelayTimer As DispatcherTimer
    Private lowerOpacityTimer As DispatcherTimer

    Public Sub showPointer()
        If Not Me.Dispatcher.CheckAccess Then
            Me.Dispatcher.Invoke(New invalidateDelegate(AddressOf showPointer))
        Else
            Me.Show()
            Me.Opacity = 1
        End If
    End Sub

    Public Sub hidePointer()
        If Not Me.Dispatcher.CheckAccess Then
            Me.Dispatcher.Invoke(New invalidateDelegate(AddressOf hidePointer))
        Else
            Me.Opacity = 0
        End If
    End Sub

    Private Sub startFadeOutPointer()
        If isFadingOutTimed Then
            fadeOutDelayTimer.Stop()
        End If

        lowerOpacityTimer = New DispatcherTimer
        AddHandler lowerOpacityTimer.Tick, AddressOf lowerOpacity
        lowerOpacityTimer.Interval = New TimeSpan(0, 0, 0, 0, 100)
        lowerOpacityTimer.Start()

        fadeOutDelayTimer.Stop()
    End Sub

    Private Sub lowerOpacity()
        If Me.Opacity <> 0 Then
            isFadingOut = True
            Me.Opacity = Me.Opacity - 0.05
        Else
            isFadingOut = False
            lowerOpacityTimer.Stop()
        End If
    End Sub

    Public Sub fadeOutPointer()
        If Not Me.Dispatcher.CheckAccess Then
            Me.Dispatcher.Invoke(New invalidateDelegate(AddressOf fadeOutPointer))
        Else
            'Start fading out pointer after 2 seconds
            If isFadingOutTimed Then
                fadeOutDelayTimer.Stop()
            End If
            If isFadingOut Then
                lowerOpacityTimer.Stop()
            End If

            fadeOutDelayTimer = New DispatcherTimer
            AddHandler fadeOutDelayTimer.Tick, AddressOf startFadeOutPointer
            fadeOutDelayTimer.Interval = New TimeSpan(0, 0, 2)
            fadeOutDelayTimer.Start()
            isFadingOutTimed = True
        End If
    End Sub

    Public Sub setPointerPosition(ByVal point As Point)

        If Not Me.Dispatcher.CheckAccess Then
            Me.Dispatcher.Invoke(New setPointerPositionDelegate(AddressOf setPointerPosition), point)
        Else
            Try
                Me.Left = point.X
                Me.Top = point.Y
            Catch ex As Exception
                Logger.add(ex.ToString)
            End Try
        End If
    End Sub

    Public Function getPointerPosition() As Point
        Return New Point(Me.Left, Me.Top)
    End Function

End Class
