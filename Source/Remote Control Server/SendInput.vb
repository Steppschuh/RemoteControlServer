Imports System.Runtime.InteropServices

Public Class SendInput

    Private Const KEYEVENTF_KEYDOWN As Integer = &H0
    Private Const KEYEVENTF_KEYUP As Integer = &H2
    Private Const KEYEVENTF_UNICODE As Long = &H4

    <DllImport("user32.dll")> _
    Private Shared Function SendInput( _
        ByVal nInputs As Integer, _
        ByVal pInputs() As INPUT, _
        ByVal cbSize As Integer) As Integer
    End Function

    <StructLayout(LayoutKind.Explicit)> _
    Private Structure INPUT
        <FieldOffset(0)> _
        Public type As Integer
        <FieldOffset(8)> _
        Public mi As MOUSEINPUT
        <FieldOffset(8)> _
        Public ki As KEYBDINPUT
        <FieldOffset(8)> _
        Public hi As HARDWAREINPUT
    End Structure

    Private Structure MOUSEINPUT
        Public dx As Integer
        Public dy As Integer
        Public mouseData As Integer
        Public dwFlags As Integer
        Public time As Integer
        Public dwExtraInfo As IntPtr
    End Structure

    Private Structure KEYBDINPUT
        Public wVk As Short
        Public wScan As Short
        Public dwFlags As Integer
        Public time As Integer
        Public dwExtraInfo As IntPtr
    End Structure

    Private Structure HARDWAREINPUT
        Public uMsg As Integer
        Public wParamL As Short
        Public wParamH As Short
    End Structure

    Public Shared Sub SendKey(ByVal key As Char)
        Dim Inpts(1) As INPUT
        'key down
        Inpts(0).type = 1
        Inpts(0).ki.wVk = Convert.ToInt16(CChar(key))
        Inpts(0).ki.dwFlags = KEYEVENTF_KEYDOWN

        'key up
        Inpts(1).type = 1
        Inpts(1).ki.wVk = Convert.ToInt16(CChar(key))
        Inpts(1).ki.dwFlags = KEYEVENTF_KEYUP
        SendInput(2, Inpts, Marshal.SizeOf(GetType(INPUT)))
    End Sub

    Public Shared Sub SendUniCodeKey(ByVal key As Int16)
        Dim Inpts(1) As INPUT
        'key down
        Inpts(0).type = 1
        Inpts(0).ki.wVk = 0
        Inpts(0).ki.wScan = key
        Inpts(0).ki.dwFlags = KEYEVENTF_UNICODE

        'key up
        Inpts(1).type = 1
        Inpts(1).ki.wVk = 0
        Inpts(1).ki.wScan = key
        Inpts(1).ki.dwFlags = KEYEVENTF_UNICODE Or KEYEVENTF_KEYUP

        SendInput(2, Inpts, Marshal.SizeOf(GetType(INPUT)))
    End Sub

End Class