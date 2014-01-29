<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class SettingsWindow
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(SettingsWindow))
        Me.TabControlMain = New System.Windows.Forms.TabControl()
        Me.tab_settings = New System.Windows.Forms.TabPage()
        Me.TabControl1 = New System.Windows.Forms.TabControl()
        Me.tab_general = New System.Windows.Forms.TabPage()
        Me.GroupBox3 = New System.Windows.Forms.GroupBox()
        Me.dropDown_backDesign = New System.Windows.Forms.ComboBox()
        Me.cb_backDesign = New System.Windows.Forms.CheckBox()
        Me.cb_showTrayNotoifications = New System.Windows.Forms.CheckBox()
        Me.cb_minimizeToTray = New System.Windows.Forms.CheckBox()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.cb_startMinimized = New System.Windows.Forms.CheckBox()
        Me.cb_autostart = New System.Windows.Forms.CheckBox()
        Me.tab_protection = New System.Windows.Forms.TabPage()
        Me.GroupBox6 = New System.Windows.Forms.GroupBox()
        Me.btn_showPin = New System.Windows.Forms.PictureBox()
        Me.tb_pin = New System.Windows.Forms.TextBox()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.cb_usePin = New System.Windows.Forms.CheckBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.label_whitelist = New System.Windows.Forms.Label()
        Me.btn_manageWhitelist = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.cb_useWhitelist = New System.Windows.Forms.CheckBox()
        Me.tab_mouse = New System.Windows.Forms.TabPage()
        Me.tab_screen = New System.Windows.Forms.TabPage()
        Me.tab_upgrade = New System.Windows.Forms.TabPage()
        Me.tab_update = New System.Windows.Forms.TabPage()
        Me.GroupBox5 = New System.Windows.Forms.GroupBox()
        Me.btn_update_changelog = New System.Windows.Forms.Button()
        Me.btn_update_install = New System.Windows.Forms.Button()
        Me.label_update_current_version = New System.Windows.Forms.Label()
        Me.label_update_available_version = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.GroupBox4 = New System.Windows.Forms.GroupBox()
        Me.btn_update_help = New System.Windows.Forms.Button()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.tab_help = New System.Windows.Forms.TabPage()
        Me.tab_log = New System.Windows.Forms.TabPage()
        Me.tb_log = New System.Windows.Forms.TextBox()
        Me.btn_appData = New System.Windows.Forms.Button()
        Me.TabControlMain.SuspendLayout()
        Me.tab_settings.SuspendLayout()
        Me.TabControl1.SuspendLayout()
        Me.tab_general.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.tab_protection.SuspendLayout()
        Me.GroupBox6.SuspendLayout()
        CType(Me.btn_showPin, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GroupBox1.SuspendLayout()
        Me.tab_update.SuspendLayout()
        Me.GroupBox5.SuspendLayout()
        Me.GroupBox4.SuspendLayout()
        Me.tab_log.SuspendLayout()
        Me.SuspendLayout()
        '
        'TabControlMain
        '
        Me.TabControlMain.Controls.Add(Me.tab_settings)
        Me.TabControlMain.Controls.Add(Me.tab_upgrade)
        Me.TabControlMain.Controls.Add(Me.tab_update)
        Me.TabControlMain.Controls.Add(Me.tab_help)
        Me.TabControlMain.Controls.Add(Me.tab_log)
        Me.TabControlMain.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TabControlMain.Location = New System.Drawing.Point(0, 0)
        Me.TabControlMain.Name = "TabControlMain"
        Me.TabControlMain.SelectedIndex = 0
        Me.TabControlMain.Size = New System.Drawing.Size(469, 329)
        Me.TabControlMain.TabIndex = 0
        '
        'tab_settings
        '
        Me.tab_settings.Controls.Add(Me.TabControl1)
        Me.tab_settings.Location = New System.Drawing.Point(4, 22)
        Me.tab_settings.Name = "tab_settings"
        Me.tab_settings.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_settings.Size = New System.Drawing.Size(461, 303)
        Me.tab_settings.TabIndex = 1
        Me.tab_settings.Text = "Settings"
        Me.tab_settings.UseVisualStyleBackColor = True
        '
        'TabControl1
        '
        Me.TabControl1.Controls.Add(Me.tab_general)
        Me.TabControl1.Controls.Add(Me.tab_protection)
        Me.TabControl1.Controls.Add(Me.tab_mouse)
        Me.TabControl1.Controls.Add(Me.tab_screen)
        Me.TabControl1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TabControl1.Location = New System.Drawing.Point(3, 3)
        Me.TabControl1.Name = "TabControl1"
        Me.TabControl1.SelectedIndex = 0
        Me.TabControl1.Size = New System.Drawing.Size(455, 297)
        Me.TabControl1.TabIndex = 0
        '
        'tab_general
        '
        Me.tab_general.Controls.Add(Me.btn_appData)
        Me.tab_general.Controls.Add(Me.GroupBox3)
        Me.tab_general.Controls.Add(Me.GroupBox2)
        Me.tab_general.Location = New System.Drawing.Point(4, 22)
        Me.tab_general.Name = "tab_general"
        Me.tab_general.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_general.Size = New System.Drawing.Size(447, 271)
        Me.tab_general.TabIndex = 2
        Me.tab_general.Text = "General"
        Me.tab_general.UseVisualStyleBackColor = True
        '
        'GroupBox3
        '
        Me.GroupBox3.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox3.Controls.Add(Me.dropDown_backDesign)
        Me.GroupBox3.Controls.Add(Me.cb_backDesign)
        Me.GroupBox3.Controls.Add(Me.cb_showTrayNotoifications)
        Me.GroupBox3.Controls.Add(Me.cb_minimizeToTray)
        Me.GroupBox3.Location = New System.Drawing.Point(6, 79)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(435, 95)
        Me.GroupBox3.TabIndex = 2
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "Appearance"
        '
        'dropDown_backDesign
        '
        Me.dropDown_backDesign.FormattingEnabled = True
        Me.dropDown_backDesign.Items.AddRange(New Object() {"Auto", "Android", "BlackBerry", "iOS"})
        Me.dropDown_backDesign.Location = New System.Drawing.Point(149, 63)
        Me.dropDown_backDesign.Name = "dropDown_backDesign"
        Me.dropDown_backDesign.Size = New System.Drawing.Size(121, 21)
        Me.dropDown_backDesign.TabIndex = 3
        Me.dropDown_backDesign.Text = "Auto"
        '
        'cb_backDesign
        '
        Me.cb_backDesign.AutoSize = True
        Me.cb_backDesign.Checked = True
        Me.cb_backDesign.CheckState = System.Windows.Forms.CheckState.Checked
        Me.cb_backDesign.Location = New System.Drawing.Point(6, 65)
        Me.cb_backDesign.Name = "cb_backDesign"
        Me.cb_backDesign.Size = New System.Drawing.Size(120, 17)
        Me.cb_backDesign.TabIndex = 2
        Me.cb_backDesign.Text = "Set server design to"
        Me.cb_backDesign.UseVisualStyleBackColor = True
        '
        'cb_showTrayNotoifications
        '
        Me.cb_showTrayNotoifications.AutoSize = True
        Me.cb_showTrayNotoifications.Checked = True
        Me.cb_showTrayNotoifications.CheckState = System.Windows.Forms.CheckState.Checked
        Me.cb_showTrayNotoifications.Location = New System.Drawing.Point(6, 42)
        Me.cb_showTrayNotoifications.Name = "cb_showTrayNotoifications"
        Me.cb_showTrayNotoifications.Size = New System.Drawing.Size(155, 17)
        Me.cb_showTrayNotoifications.TabIndex = 1
        Me.cb_showTrayNotoifications.Text = "Show tray icon notifications"
        Me.cb_showTrayNotoifications.UseVisualStyleBackColor = True
        '
        'cb_minimizeToTray
        '
        Me.cb_minimizeToTray.AutoSize = True
        Me.cb_minimizeToTray.Checked = True
        Me.cb_minimizeToTray.CheckState = System.Windows.Forms.CheckState.Checked
        Me.cb_minimizeToTray.Location = New System.Drawing.Point(6, 19)
        Me.cb_minimizeToTray.Name = "cb_minimizeToTray"
        Me.cb_minimizeToTray.Size = New System.Drawing.Size(153, 17)
        Me.cb_minimizeToTray.TabIndex = 0
        Me.cb_minimizeToTray.Text = "Minimize server to tray icon"
        Me.cb_minimizeToTray.UseVisualStyleBackColor = True
        '
        'GroupBox2
        '
        Me.GroupBox2.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox2.Controls.Add(Me.cb_startMinimized)
        Me.GroupBox2.Controls.Add(Me.cb_autostart)
        Me.GroupBox2.Location = New System.Drawing.Point(6, 6)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(435, 67)
        Me.GroupBox2.TabIndex = 1
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Startup"
        '
        'cb_startMinimized
        '
        Me.cb_startMinimized.AutoSize = True
        Me.cb_startMinimized.Location = New System.Drawing.Point(6, 42)
        Me.cb_startMinimized.Name = "cb_startMinimized"
        Me.cb_startMinimized.Size = New System.Drawing.Size(128, 17)
        Me.cb_startMinimized.TabIndex = 1
        Me.cb_startMinimized.Text = "Start server minimized"
        Me.cb_startMinimized.UseVisualStyleBackColor = True
        '
        'cb_autostart
        '
        Me.cb_autostart.AutoSize = True
        Me.cb_autostart.Location = New System.Drawing.Point(6, 19)
        Me.cb_autostart.Name = "cb_autostart"
        Me.cb_autostart.Size = New System.Drawing.Size(184, 17)
        Me.cb_autostart.TabIndex = 0
        Me.cb_autostart.Text = "Start server when Windows starts"
        Me.cb_autostart.UseVisualStyleBackColor = True
        '
        'tab_protection
        '
        Me.tab_protection.Controls.Add(Me.GroupBox6)
        Me.tab_protection.Controls.Add(Me.GroupBox1)
        Me.tab_protection.Location = New System.Drawing.Point(4, 22)
        Me.tab_protection.Name = "tab_protection"
        Me.tab_protection.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_protection.Size = New System.Drawing.Size(447, 271)
        Me.tab_protection.TabIndex = 3
        Me.tab_protection.Text = "Protection"
        Me.tab_protection.UseVisualStyleBackColor = True
        '
        'GroupBox6
        '
        Me.GroupBox6.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox6.Controls.Add(Me.btn_showPin)
        Me.GroupBox6.Controls.Add(Me.tb_pin)
        Me.GroupBox6.Controls.Add(Me.Label6)
        Me.GroupBox6.Controls.Add(Me.cb_usePin)
        Me.GroupBox6.Location = New System.Drawing.Point(6, 112)
        Me.GroupBox6.Name = "GroupBox6"
        Me.GroupBox6.Size = New System.Drawing.Size(435, 100)
        Me.GroupBox6.TabIndex = 4
        Me.GroupBox6.TabStop = False
        Me.GroupBox6.Text = "Pin"
        '
        'btn_showPin
        '
        Me.btn_showPin.Cursor = System.Windows.Forms.Cursors.Help
        Me.btn_showPin.Image = Global.RemoteControlServer.My.Resources.Resources.ic_action_search
        Me.btn_showPin.Location = New System.Drawing.Point(115, 61)
        Me.btn_showPin.Name = "btn_showPin"
        Me.btn_showPin.Size = New System.Drawing.Size(29, 26)
        Me.btn_showPin.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.btn_showPin.TabIndex = 3
        Me.btn_showPin.TabStop = False
        '
        'tb_pin
        '
        Me.tb_pin.Location = New System.Drawing.Point(8, 64)
        Me.tb_pin.Name = "tb_pin"
        Me.tb_pin.Size = New System.Drawing.Size(100, 20)
        Me.tb_pin.TabIndex = 2
        Me.tb_pin.Text = "0000"
        Me.tb_pin.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        Me.tb_pin.UseSystemPasswordChar = True
        '
        'Label6
        '
        Me.Label6.Location = New System.Drawing.Point(10, 16)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(419, 22)
        Me.Label6.TabIndex = 1
        Me.Label6.Text = "If enabled, devices can only connect by entering the correct pin."
        '
        'cb_usePin
        '
        Me.cb_usePin.AutoSize = True
        Me.cb_usePin.Location = New System.Drawing.Point(8, 41)
        Me.cb_usePin.Name = "cb_usePin"
        Me.cb_usePin.Size = New System.Drawing.Size(76, 17)
        Me.cb_usePin.TabIndex = 0
        Me.cb_usePin.Text = "Enable pin"
        Me.cb_usePin.UseVisualStyleBackColor = True
        '
        'GroupBox1
        '
        Me.GroupBox1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox1.Controls.Add(Me.label_whitelist)
        Me.GroupBox1.Controls.Add(Me.btn_manageWhitelist)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.cb_useWhitelist)
        Me.GroupBox1.Location = New System.Drawing.Point(6, 6)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(435, 100)
        Me.GroupBox1.TabIndex = 0
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Whitelist"
        '
        'label_whitelist
        '
        Me.label_whitelist.AutoSize = True
        Me.label_whitelist.ForeColor = System.Drawing.Color.DimGray
        Me.label_whitelist.Location = New System.Drawing.Point(131, 42)
        Me.label_whitelist.Name = "label_whitelist"
        Me.label_whitelist.Size = New System.Drawing.Size(105, 13)
        Me.label_whitelist.TabIndex = 3
        Me.label_whitelist.Text = "0 devices whitelisted"
        '
        'btn_manageWhitelist
        '
        Me.btn_manageWhitelist.Location = New System.Drawing.Point(6, 64)
        Me.btn_manageWhitelist.Name = "btn_manageWhitelist"
        Me.btn_manageWhitelist.Size = New System.Drawing.Size(145, 23)
        Me.btn_manageWhitelist.TabIndex = 2
        Me.btn_manageWhitelist.Text = "Manage devices"
        Me.btn_manageWhitelist.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(10, 16)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(419, 22)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "If enabled, only commands from known IP addresses will be accepted."
        '
        'cb_useWhitelist
        '
        Me.cb_useWhitelist.AutoSize = True
        Me.cb_useWhitelist.Location = New System.Drawing.Point(8, 41)
        Me.cb_useWhitelist.Name = "cb_useWhitelist"
        Me.cb_useWhitelist.Size = New System.Drawing.Size(98, 17)
        Me.cb_useWhitelist.TabIndex = 0
        Me.cb_useWhitelist.Text = "Enable whilelist"
        Me.cb_useWhitelist.UseVisualStyleBackColor = True
        '
        'tab_mouse
        '
        Me.tab_mouse.Location = New System.Drawing.Point(4, 22)
        Me.tab_mouse.Name = "tab_mouse"
        Me.tab_mouse.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_mouse.Size = New System.Drawing.Size(447, 271)
        Me.tab_mouse.TabIndex = 0
        Me.tab_mouse.Text = "Mouse"
        Me.tab_mouse.UseVisualStyleBackColor = True
        '
        'tab_screen
        '
        Me.tab_screen.Location = New System.Drawing.Point(4, 22)
        Me.tab_screen.Name = "tab_screen"
        Me.tab_screen.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_screen.Size = New System.Drawing.Size(447, 271)
        Me.tab_screen.TabIndex = 1
        Me.tab_screen.Text = "Screen"
        Me.tab_screen.UseVisualStyleBackColor = True
        '
        'tab_upgrade
        '
        Me.tab_upgrade.Location = New System.Drawing.Point(4, 22)
        Me.tab_upgrade.Name = "tab_upgrade"
        Me.tab_upgrade.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_upgrade.Size = New System.Drawing.Size(461, 303)
        Me.tab_upgrade.TabIndex = 2
        Me.tab_upgrade.Text = "Upgrade"
        Me.tab_upgrade.UseVisualStyleBackColor = True
        '
        'tab_update
        '
        Me.tab_update.Controls.Add(Me.GroupBox5)
        Me.tab_update.Controls.Add(Me.GroupBox4)
        Me.tab_update.Location = New System.Drawing.Point(4, 22)
        Me.tab_update.Name = "tab_update"
        Me.tab_update.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_update.Size = New System.Drawing.Size(461, 303)
        Me.tab_update.TabIndex = 4
        Me.tab_update.Text = "Update"
        Me.tab_update.UseVisualStyleBackColor = True
        '
        'GroupBox5
        '
        Me.GroupBox5.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox5.Controls.Add(Me.btn_update_changelog)
        Me.GroupBox5.Controls.Add(Me.btn_update_install)
        Me.GroupBox5.Controls.Add(Me.label_update_current_version)
        Me.GroupBox5.Controls.Add(Me.label_update_available_version)
        Me.GroupBox5.Controls.Add(Me.Label5)
        Me.GroupBox5.Controls.Add(Me.Label4)
        Me.GroupBox5.Location = New System.Drawing.Point(6, 6)
        Me.GroupBox5.Name = "GroupBox5"
        Me.GroupBox5.Size = New System.Drawing.Size(449, 91)
        Me.GroupBox5.TabIndex = 3
        Me.GroupBox5.TabStop = False
        Me.GroupBox5.Text = "Update"
        '
        'btn_update_changelog
        '
        Me.btn_update_changelog.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.btn_update_changelog.Location = New System.Drawing.Point(6, 62)
        Me.btn_update_changelog.Name = "btn_update_changelog"
        Me.btn_update_changelog.Size = New System.Drawing.Size(212, 23)
        Me.btn_update_changelog.TabIndex = 5
        Me.btn_update_changelog.Text = "Show changelog"
        Me.btn_update_changelog.UseVisualStyleBackColor = True
        '
        'btn_update_install
        '
        Me.btn_update_install.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btn_update_install.Location = New System.Drawing.Point(231, 62)
        Me.btn_update_install.Name = "btn_update_install"
        Me.btn_update_install.Size = New System.Drawing.Size(212, 23)
        Me.btn_update_install.TabIndex = 4
        Me.btn_update_install.Text = "Install the latest Version"
        Me.btn_update_install.UseVisualStyleBackColor = True
        '
        'label_update_current_version
        '
        Me.label_update_current_version.AutoSize = True
        Me.label_update_current_version.Location = New System.Drawing.Point(189, 36)
        Me.label_update_current_version.Name = "label_update_current_version"
        Me.label_update_current_version.Size = New System.Drawing.Size(53, 13)
        Me.label_update_current_version.TabIndex = 3
        Me.label_update_current_version.Text = "Unknown"
        '
        'label_update_available_version
        '
        Me.label_update_available_version.AutoSize = True
        Me.label_update_available_version.Location = New System.Drawing.Point(189, 20)
        Me.label_update_available_version.Name = "label_update_available_version"
        Me.label_update_available_version.Size = New System.Drawing.Size(53, 13)
        Me.label_update_available_version.TabIndex = 2
        Me.label_update_available_version.Text = "Unknown"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(6, 36)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(129, 13)
        Me.Label5.TabIndex = 1
        Me.Label5.Text = "Currently installed version:"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(6, 20)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(121, 13)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Latest available version:"
        '
        'GroupBox4
        '
        Me.GroupBox4.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox4.Controls.Add(Me.btn_update_help)
        Me.GroupBox4.Controls.Add(Me.Label3)
        Me.GroupBox4.Location = New System.Drawing.Point(6, 103)
        Me.GroupBox4.Name = "GroupBox4"
        Me.GroupBox4.Size = New System.Drawing.Size(449, 125)
        Me.GroupBox4.TabIndex = 2
        Me.GroupBox4.TabStop = False
        Me.GroupBox4.Text = "Info"
        '
        'btn_update_help
        '
        Me.btn_update_help.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btn_update_help.Location = New System.Drawing.Point(9, 96)
        Me.btn_update_help.Name = "btn_update_help"
        Me.btn_update_help.Size = New System.Drawing.Size(434, 23)
        Me.btn_update_help.TabIndex = 1
        Me.btn_update_help.Text = "How to update the Remote Control Server"
        Me.btn_update_help.UseVisualStyleBackColor = True
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(6, 16)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(437, 76)
        Me.Label3.TabIndex = 0
        Me.Label3.Text = resources.GetString("Label3.Text")
        '
        'tab_help
        '
        Me.tab_help.Location = New System.Drawing.Point(4, 22)
        Me.tab_help.Name = "tab_help"
        Me.tab_help.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_help.Size = New System.Drawing.Size(461, 303)
        Me.tab_help.TabIndex = 3
        Me.tab_help.Text = "Help"
        Me.tab_help.UseVisualStyleBackColor = True
        '
        'tab_log
        '
        Me.tab_log.Controls.Add(Me.tb_log)
        Me.tab_log.Location = New System.Drawing.Point(4, 22)
        Me.tab_log.Name = "tab_log"
        Me.tab_log.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_log.Size = New System.Drawing.Size(461, 303)
        Me.tab_log.TabIndex = 0
        Me.tab_log.Text = "Log"
        Me.tab_log.UseVisualStyleBackColor = True
        '
        'tb_log
        '
        Me.tb_log.BackColor = System.Drawing.Color.FromArgb(CType(CType(30, Byte), Integer), CType(CType(30, Byte), Integer), CType(CType(30, Byte), Integer))
        Me.tb_log.Dock = System.Windows.Forms.DockStyle.Fill
        Me.tb_log.ForeColor = System.Drawing.Color.White
        Me.tb_log.Location = New System.Drawing.Point(3, 3)
        Me.tb_log.Multiline = True
        Me.tb_log.Name = "tb_log"
        Me.tb_log.ReadOnly = True
        Me.tb_log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.tb_log.Size = New System.Drawing.Size(455, 297)
        Me.tb_log.TabIndex = 0
        Me.tb_log.Text = "Log"
        '
        'btn_appData
        '
        Me.btn_appData.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btn_appData.Location = New System.Drawing.Point(276, 242)
        Me.btn_appData.Name = "btn_appData"
        Me.btn_appData.Size = New System.Drawing.Size(165, 23)
        Me.btn_appData.TabIndex = 3
        Me.btn_appData.Text = "Open app data folder"
        Me.btn_appData.UseVisualStyleBackColor = True
        '
        'SettingsWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(469, 329)
        Me.Controls.Add(Me.TabControlMain)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "SettingsWindow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Advanced - Remote Control Server "
        Me.TabControlMain.ResumeLayout(False)
        Me.tab_settings.ResumeLayout(False)
        Me.TabControl1.ResumeLayout(False)
        Me.tab_general.ResumeLayout(False)
        Me.GroupBox3.ResumeLayout(False)
        Me.GroupBox3.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.tab_protection.ResumeLayout(False)
        Me.GroupBox6.ResumeLayout(False)
        Me.GroupBox6.PerformLayout()
        CType(Me.btn_showPin, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.tab_update.ResumeLayout(False)
        Me.GroupBox5.ResumeLayout(False)
        Me.GroupBox5.PerformLayout()
        Me.GroupBox4.ResumeLayout(False)
        Me.tab_log.ResumeLayout(False)
        Me.tab_log.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents TabControlMain As System.Windows.Forms.TabControl
    Friend WithEvents tab_log As System.Windows.Forms.TabPage
    Friend WithEvents tab_settings As System.Windows.Forms.TabPage
    Friend WithEvents tb_log As System.Windows.Forms.TextBox
    Friend WithEvents tab_upgrade As System.Windows.Forms.TabPage
    Friend WithEvents tab_help As System.Windows.Forms.TabPage
    Friend WithEvents TabControl1 As System.Windows.Forms.TabControl
    Friend WithEvents tab_mouse As System.Windows.Forms.TabPage
    Friend WithEvents tab_screen As System.Windows.Forms.TabPage
    Friend WithEvents tab_general As System.Windows.Forms.TabPage
    Friend WithEvents tab_protection As System.Windows.Forms.TabPage
    Friend WithEvents tab_update As System.Windows.Forms.TabPage
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents cb_useWhitelist As System.Windows.Forms.CheckBox
    Friend WithEvents btn_manageWhitelist As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents label_whitelist As System.Windows.Forms.Label
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents cb_autostart As System.Windows.Forms.CheckBox
    Friend WithEvents cb_startMinimized As System.Windows.Forms.CheckBox
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents cb_showTrayNotoifications As System.Windows.Forms.CheckBox
    Friend WithEvents cb_minimizeToTray As System.Windows.Forms.CheckBox
    Friend WithEvents dropDown_backDesign As System.Windows.Forms.ComboBox
    Friend WithEvents cb_backDesign As System.Windows.Forms.CheckBox
    Friend WithEvents GroupBox4 As System.Windows.Forms.GroupBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents btn_update_help As System.Windows.Forms.Button
    Friend WithEvents GroupBox5 As System.Windows.Forms.GroupBox
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents label_update_current_version As System.Windows.Forms.Label
    Friend WithEvents label_update_available_version As System.Windows.Forms.Label
    Friend WithEvents btn_update_install As System.Windows.Forms.Button
    Friend WithEvents btn_update_changelog As System.Windows.Forms.Button
    Friend WithEvents GroupBox6 As System.Windows.Forms.GroupBox
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents cb_usePin As System.Windows.Forms.CheckBox
    Friend WithEvents tb_pin As System.Windows.Forms.TextBox
    Friend WithEvents btn_showPin As System.Windows.Forms.PictureBox
    Friend WithEvents btn_appData As System.Windows.Forms.Button
End Class
