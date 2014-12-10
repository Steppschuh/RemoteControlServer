Imports System.Xml
Imports System.IO
Imports System.Text

Module Settings

    'Loads, holds and saves application settings

    Public Const SETTINGS_PATH As String = "config.xml"

    Public Const OS_AUTO As Byte = 0
    Public Const OS_ANDROID As Byte = 1
    Public Const OS_BLACKBERRY As Byte = 2
    Public Const OS_IOS As Byte = 3
    Public Const OS_GENERAL As Byte = 4

    'General
    Public autoStart As Boolean = False
    Public startMinimized As Boolean = False
    Public minimizeToTray As Boolean = True
    Public showTrayNotoifications As Boolean = True

    Public showGuide As Boolean = True
    Public backDesign As Byte = OS_AUTO
    Public lastBackDesign As Byte = OS_AUTO

    'Authentication
    Public useWhiteList As Boolean = False
    Public usePin As Boolean = False
    Public pin As String = "0000"
    Public whitelistedIps As List(Of String)

    'Mouse
    Public mouseSensitivity As Single = 5
    Public mouseAcceleration As Single = 1.5
    Public motionFilter As Byte = 1
    Public motionAcceleration As Byte = 3

    'Screen
    Public screenQuality As Byte = 40
    Public screenQualityFull As Byte = 60
    Public screenScale As Single = 0.5
    Public screenScaleFull As Single = 1
    Public screenBlackWhite As Boolean = False

    'Slideshow
    Public clickOnLaserUp As Boolean = False
    Public pointerDesign As Byte = 0
    Public cropBlackBorder As Boolean = True

    'Media
    Public defaultMediaPlayer As String = ""

    'Misc
    Public serialPortName As String = "COM3"
    Public serialCommands As Boolean = False
    Public updateAmbientColor As Boolean = False

    Public Sub loadSettings()
        Try
            Logger.add("Loading settings")
            whitelistedIps = New List(Of String)
            readSettingsFromFile()
        Catch ex As Exception

        End Try
    End Sub

    Public Sub saveSettings()
        Try
            saveSettingsToFile()
        Catch ex As Exception

        End Try
    End Sub

    Private Sub readSettingsFromFile()
        Dim path As String = getConfigPath()
        Dim sr As StreamReader
        Dim xmlString As String = ""

        Try
            If My.Computer.FileSystem.FileExists(path) Then
                sr = New StreamReader(path, Text.Encoding.UTF8)
                xmlString = sr.ReadToEnd()
                parseSettings(xmlString)
                parseWhitelist(xmlString)
            Else
                Throw New FileNotFoundException
            End If
        Catch ex As Exception
            Logger.add("Error while reading settings file from:" & vbNewLine & path)
            Logger.add(ex.ToString)
        End Try
    End Sub

    Private Sub saveSettingsToFile()
        Dim path As String = getConfigPath()
        Dim sw As StreamWriter
        Dim xmlString As String = ""

        Try
            'Check if directory exists
            If Not My.Computer.FileSystem.DirectoryExists(getAppDataDirectory()) Then
                My.Computer.FileSystem.CreateDirectory(getAppDataDirectory())
            End If

            sw = New StreamWriter(path, False, Text.Encoding.UTF8)

            sw.Write("<?xml version=" & Chr(34) & "1.0" & Chr(34) & " encoding=" & Chr(34) & "utf-8" & Chr(34) & "?>")
            sw.WriteLine()
            sw.Write("<settings>")
            sw.WriteLine()

            'General
            appendSetting("autoStart", Converter.boolToString(autoStart), sw)
            appendSetting("startMinimized", Converter.boolToString(startMinimized), sw)
            appendSetting("minimizeToTray", Converter.boolToString(minimizeToTray), sw)
            appendSetting("showTrayNotoifications", Converter.boolToString(showTrayNotoifications), sw)
            appendSetting("showGuide", Converter.boolToString(showGuide), sw)
            sw.WriteLine()

            'Background
            appendSetting("backDesign", backDesign.ToString, sw)
            appendSetting("lastBackDesign", lastBackDesign.ToString, sw)
            sw.WriteLine()

            'Authentication
            appendSetting("useWhiteList", Converter.boolToString(useWhiteList), sw)
            appendSetting("usePin", Converter.boolToString(usePin), sw)
            appendSetting("pin", pin, sw)
            sw.WriteLine()

            'Mouse and Pointer
            appendSetting("mouseSensitivity", mouseSensitivity.ToString, sw)
            appendSetting("mouseAcceleration", mouseAcceleration.ToString, sw)
            appendSetting("motionFilter", motionFilter.ToString, sw)
            appendSetting("motionAccel", motionAcceleration.ToString, sw)
            sw.WriteLine()

            'Screen
            appendSetting("screenQuality", screenQuality.ToString, sw)
            appendSetting("screenQualityFull", screenQualityFull.ToString, sw)
            appendSetting("screenScale", screenScale.ToString, sw)
            appendSetting("screenScaleFull", screenScaleFull.ToString, sw)
            appendSetting("screenBlackWhite", Converter.boolToString(screenBlackWhite), sw)
            sw.WriteLine()

            'Slideshow
            appendSetting("clickOnLaserUp", Converter.boolToString(clickOnLaserUp), sw)
            appendSetting("pointerDesign", pointerDesign.ToString, sw)
            appendSetting("cropBlackBorder", Converter.boolToString(cropBlackBorder), sw)
            sw.WriteLine()

            'Media
            appendSetting("defaultMediaPlayer", defaultMediaPlayer, sw)
            sw.WriteLine()

            'Misc
            appendSetting("serialCommands", Converter.boolToString(serialCommands), sw)
            appendSetting("serialPortName", serialPortName, sw)
            appendSetting("updateAmbientColor", Converter.boolToString(updateAmbientColor), sw)
            sw.WriteLine()

            'Whitelist
            sw.Write("  <whitelist>")
            sw.WriteLine()

            For Each appIp As String In whitelistedIps
                appendWhitelistIp(appIp, sw)
            Next

            sw.Write("  </whitelist>")
            sw.WriteLine()

            sw.Write("</settings>")
            sw.Flush()
            sw.Close()

        Catch ex As Exception
            Logger.add("Error while saving settings file to:" & vbNewLine & path)
            Logger.add(ex.ToString)
        End Try
    End Sub

    Private Sub parseSettings(ByVal xmlString As String)
        Try
            Logger.add("Parsing config file")

            Dim xmldoc As New XmlDocument()
            xmldoc.LoadXml(xmlString)

            Dim xml_nodelist As XmlNodeList = xmldoc.GetElementsByTagName("/settings")
            Dim itemName, itemValue As String

            For Each mxmlnode As XmlNode In xmldoc.GetElementsByTagName("setting")
                Try
                    itemName = mxmlnode.Attributes.ItemOf("name").InnerText
                    itemValue = mxmlnode.Attributes.ItemOf("value").InnerText
                    assignSetting(itemName, itemValue)
                Catch ex As Exception
                    Logger.add(ex.ToString)
                End Try
            Next
            Logger.add("Settings restored")
        Catch ex As Exception
            Logger.add("Error while parsing config file:")
            Logger.add(ex.ToString)
        End Try
    End Sub

    Private Sub assignSetting(ByVal name As String, ByVal value As String)
        'General
        If name.Equals("autoStart") Then
            autoStart = Converter.stringToBool(value)
            setAutostart(autoStart)
        ElseIf name.Equals("startMinimized") Then
            startMinimized = Converter.stringToBool(value)
        ElseIf name.Equals("minimizeToTray") Then
            minimizeToTray = Converter.stringToBool(value)
        ElseIf name.Equals("showTrayNotoifications") Then
            showTrayNotoifications = Converter.stringToBool(value)
        ElseIf name.Equals("showGuide") Then
            showGuide = Converter.stringToBool(value)

            'Background
        ElseIf name.Equals("backDesign") Then
            backDesign = Integer.Parse(value)
        ElseIf name.Equals("lastBackDesign") Then
            lastBackDesign = Integer.Parse(value)

            'Authentication
        ElseIf name.Equals("useWhiteList") Then
            useWhiteList = Converter.stringToBool(value)
        ElseIf name.Equals("usePin") Then
            usePin = Converter.stringToBool(value)
        ElseIf name.Equals("pin") Then
            pin = value

            'Mouse and pointer
        ElseIf name.Equals("mouseSensitivity") Then
            mouseSensitivity = Single.Parse(value)
        ElseIf name.Equals("mouseAcceleration") Then
            mouseAcceleration = Single.Parse(value)
        ElseIf name.Equals("motionFilter") Then
            motionFilter = Integer.Parse(value)
        ElseIf name.Equals("motionAccel") Then
            motionAcceleration = Integer.Parse(value)

            'Screen
        ElseIf name.Equals("screenQuality") Then
            screenQuality = Integer.Parse(value)
        ElseIf name.Equals("screenQualityFull") Then
            screenQualityFull = Integer.Parse(value)
        ElseIf name.Equals("screenScale") Then
            screenScale = Single.Parse(value)
        ElseIf name.Equals("screenScaleFull") Then
            screenScaleFull = 1
        ElseIf name.Equals("screenBlackWhite") Then
            screenBlackWhite = Converter.stringToBool(value)

            'Slideshow
        ElseIf name.Equals("clickOnLaserUp") Then
            clickOnLaserUp = Converter.stringToBool(value)
        ElseIf name.Equals("pointerDesign") Then
            pointerDesign = Integer.Parse(value)
        ElseIf name.Equals("cropBlackBorder") Then
            cropBlackBorder = Converter.stringToBool(value)

            'Media
        ElseIf name.Equals("defaultMediaPlayer") Then
            defaultMediaPlayer = value

            'Misc
        ElseIf name.Equals("serialCommands") Then
            serialCommands = Converter.stringToBool(value)
        ElseIf name.Equals("serialPortName") Then
            serialPortName = value
        ElseIf name.Equals("updateAmbientColor") Then
            updateAmbientColor = Converter.stringToBool(value)

        Else
            Logger.add("Unknown config entry: " & name)
        End If
    End Sub

    Private Sub appendSetting(ByVal name As String, ByVal value As String, ByVal sw As StreamWriter)
        sw.Write("  <setting name=" & Chr(34) & name & Chr(34) & " value=" & Chr(34) & value & Chr(34) & "/>")
        sw.WriteLine()
    End Sub

    Private Sub appendWhitelistIp(ByVal ip As String, ByVal sw As StreamWriter)
        sw.Write("    <app ip=" & Chr(34) & ip & Chr(34) & "/>")
        sw.WriteLine()
    End Sub

    Private Sub parseWhitelist(ByVal xmlString As String)
        Try
            Logger.add("Parsing whitelist")

            Dim xmldoc As New XmlDocument()
            xmldoc.LoadXml(xmlString)

            Dim xml_nodelist As XmlNodeList = xmldoc.GetElementsByTagName("/whitelist")
            Dim appIP As String

            For Each mxmlnode As XmlNode In xmldoc.GetElementsByTagName("app")
                Try
                    appIP = mxmlnode.Attributes.ItemOf("ip").InnerText
                    whitelistedIps.Add(appIP)
                Catch ex As Exception
                    Logger.add(ex.ToString)
                End Try
            Next
            Logger.add("Whitelist restored, " & whitelistedIps.Count & " IPs saved")
        Catch ex As Exception
            Logger.add("Error while parsing whitelist:")
            Logger.add(ex.ToString)
        End Try
        If whitelistedIps.Count = 0 Then
            'Add one IP to show the syntax
            whitelistedIps.Add("127.0.0.1")
        End If
    End Sub

    Public Function getAppDataDirectory() As String
        Return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) & "\Remote Control Server"
    End Function

    Public Function getConfigPath() As String
        Return getAppDataDirectory() & "\" & SETTINGS_PATH
    End Function

    Public Function setAutostart(ByVal value As Boolean)
        Try
            Dim currentValue As String
            Dim key As Microsoft.Win32.RegistryKey

            key = My.Computer.Registry.LocalMachine.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Run", False)
            currentValue = key.GetValue("Remote Control Server")

            If value Then
                If Not currentValue = Nothing Then
                    If currentValue.Equals(System.Reflection.Assembly.GetExecutingAssembly().Location) Then
                        Return True
                    End If
                End If
                key = My.Computer.Registry.LocalMachine.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Run", True)
                key.SetValue("Remote Control Server", System.Reflection.Assembly.GetExecutingAssembly().Location)
            Else
                If Not currentValue = Nothing Then
                    key = My.Computer.Registry.LocalMachine.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Run", True)
                    key.DeleteValue("Remote Control Server")
                End If
            End If
            Return True
        Catch ex As Exception
            Logger.add("Unable to set autostart value, please start the server with administrator rights." & vbNewLine & ex.ToString)
        End Try
        Return False
    End Function

End Module
