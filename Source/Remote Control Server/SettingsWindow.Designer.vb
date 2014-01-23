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
        Me.ComboBox1 = New System.Windows.Forms.ComboBox()
        Me.CheckBox6 = New System.Windows.Forms.CheckBox()
        Me.CheckBox4 = New System.Windows.Forms.CheckBox()
        Me.CheckBox5 = New System.Windows.Forms.CheckBox()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.CheckBox3 = New System.Windows.Forms.CheckBox()
        Me.CheckBox2 = New System.Windows.Forms.CheckBox()
        Me.tab_protection = New System.Windows.Forms.TabPage()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.CheckBox1 = New System.Windows.Forms.CheckBox()
        Me.tab_mouse = New System.Windows.Forms.TabPage()
        Me.tab_screen = New System.Windows.Forms.TabPage()
        Me.tab_upgrade = New System.Windows.Forms.TabPage()
        Me.tab_update = New System.Windows.Forms.TabPage()
        Me.tab_help = New System.Windows.Forms.TabPage()
        Me.tab_log = New System.Windows.Forms.TabPage()
        Me.tb_log = New System.Windows.Forms.TextBox()
        Me.GroupBox4 = New System.Windows.Forms.GroupBox()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.btn_update_help = New System.Windows.Forms.Button()
        Me.GroupBox5 = New System.Windows.Forms.GroupBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.label_update_current_version = New System.Windows.Forms.Label()
        Me.label_update_available_version = New System.Windows.Forms.Label()
        Me.btn_update_install = New System.Windows.Forms.Button()
        Me.btn_update_changelog = New System.Windows.Forms.Button()
        Me.TabControlMain.SuspendLayout()
        Me.tab_settings.SuspendLayout()
        Me.TabControl1.SuspendLayout()
        Me.tab_general.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.tab_protection.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.tab_update.SuspendLayout()
        Me.tab_log.SuspendLayout()
        Me.GroupBox4.SuspendLayout()
        Me.GroupBox5.SuspendLayout()
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
        Me.GroupBox3.Controls.Add(Me.ComboBox1)
        Me.GroupBox3.Controls.Add(Me.CheckBox6)
        Me.GroupBox3.Controls.Add(Me.CheckBox4)
        Me.GroupBox3.Controls.Add(Me.CheckBox5)
        Me.GroupBox3.Location = New System.Drawing.Point(6, 79)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(435, 95)
        Me.GroupBox3.TabIndex = 2
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "Appearance"
        '
        'ComboBox1
        '
        Me.ComboBox1.FormattingEnabled = True
        Me.ComboBox1.Items.AddRange(New Object() {"Auto", "Android", "BlackBerry", "iOS"})
        Me.ComboBox1.Location = New System.Drawing.Point(149, 63)
        Me.ComboBox1.Name = "ComboBox1"
        Me.ComboBox1.Size = New System.Drawing.Size(121, 21)
        Me.ComboBox1.TabIndex = 3
        Me.ComboBox1.Text = "Auto"
        '
        'CheckBox6
        '
        Me.CheckBox6.AutoSize = True
        Me.CheckBox6.Checked = True
        Me.CheckBox6.CheckState = System.Windows.Forms.CheckState.Checked
        Me.CheckBox6.Location = New System.Drawing.Point(6, 65)
        Me.CheckBox6.Name = "CheckBox6"
        Me.CheckBox6.Size = New System.Drawing.Size(120, 17)
        Me.CheckBox6.TabIndex = 2
        Me.CheckBox6.Text = "Set server design to"
        Me.CheckBox6.UseVisualStyleBackColor = True
        '
        'CheckBox4
        '
        Me.CheckBox4.AutoSize = True
        Me.CheckBox4.Checked = True
        Me.CheckBox4.CheckState = System.Windows.Forms.CheckState.Checked
        Me.CheckBox4.Location = New System.Drawing.Point(6, 42)
        Me.CheckBox4.Name = "CheckBox4"
        Me.CheckBox4.Size = New System.Drawing.Size(155, 17)
        Me.CheckBox4.TabIndex = 1
        Me.CheckBox4.Text = "Show tray icon notifications"
        Me.CheckBox4.UseVisualStyleBackColor = True
        '
        'CheckBox5
        '
        Me.CheckBox5.AutoSize = True
        Me.CheckBox5.Checked = True
        Me.CheckBox5.CheckState = System.Windows.Forms.CheckState.Checked
        Me.CheckBox5.Location = New System.Drawing.Point(6, 19)
        Me.CheckBox5.Name = "CheckBox5"
        Me.CheckBox5.Size = New System.Drawing.Size(153, 17)
        Me.CheckBox5.TabIndex = 0
        Me.CheckBox5.Text = "Minimize server to tray icon"
        Me.CheckBox5.UseVisualStyleBackColor = True
        '
        'GroupBox2
        '
        Me.GroupBox2.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox2.Controls.Add(Me.CheckBox3)
        Me.GroupBox2.Controls.Add(Me.CheckBox2)
        Me.GroupBox2.Location = New System.Drawing.Point(6, 6)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(435, 67)
        Me.GroupBox2.TabIndex = 1
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Startup"
        '
        'CheckBox3
        '
        Me.CheckBox3.AutoSize = True
        Me.CheckBox3.Location = New System.Drawing.Point(6, 42)
        Me.CheckBox3.Name = "CheckBox3"
        Me.CheckBox3.Size = New System.Drawing.Size(128, 17)
        Me.CheckBox3.TabIndex = 1
        Me.CheckBox3.Text = "Start server minimized"
        Me.CheckBox3.UseVisualStyleBackColor = True
        '
        'CheckBox2
        '
        Me.CheckBox2.AutoSize = True
        Me.CheckBox2.Location = New System.Drawing.Point(6, 19)
        Me.CheckBox2.Name = "CheckBox2"
        Me.CheckBox2.Size = New System.Drawing.Size(184, 17)
        Me.CheckBox2.TabIndex = 0
        Me.CheckBox2.Text = "Start server when Windows starts"
        Me.CheckBox2.UseVisualStyleBackColor = True
        '
        'tab_protection
        '
        Me.tab_protection.Controls.Add(Me.GroupBox1)
        Me.tab_protection.Location = New System.Drawing.Point(4, 22)
        Me.tab_protection.Name = "tab_protection"
        Me.tab_protection.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_protection.Size = New System.Drawing.Size(447, 271)
        Me.tab_protection.TabIndex = 3
        Me.tab_protection.Text = "Protection"
        Me.tab_protection.UseVisualStyleBackColor = True
        '
        'GroupBox1
        '
        Me.GroupBox1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.Button1)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.CheckBox1)
        Me.GroupBox1.Location = New System.Drawing.Point(6, 6)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(435, 100)
        Me.GroupBox1.TabIndex = 0
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Whitelist"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.ForeColor = System.Drawing.Color.DimGray
        Me.Label2.Location = New System.Drawing.Point(131, 42)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(105, 13)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "0 devices whitelisted"
        '
        'Button1
        '
        Me.Button1.Location = New System.Drawing.Point(6, 64)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(145, 23)
        Me.Button1.TabIndex = 2
        Me.Button1.Text = "Manage devices"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(10, 16)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(419, 22)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "If enabled, only commands from known IP addresses will be accepted."
        '
        'CheckBox1
        '
        Me.CheckBox1.AutoSize = True
        Me.CheckBox1.Location = New System.Drawing.Point(8, 41)
        Me.CheckBox1.Name = "CheckBox1"
        Me.CheckBox1.Size = New System.Drawing.Size(98, 17)
        Me.CheckBox1.TabIndex = 0
        Me.CheckBox1.Text = "Enable whilelist"
        Me.CheckBox1.UseVisualStyleBackColor = True
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
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(6, 16)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(437, 76)
        Me.Label3.TabIndex = 0
        Me.Label3.Text = resources.GetString("Label3.Text")
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
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(6, 20)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(121, 13)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Latest available version:"
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
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.tab_update.ResumeLayout(False)
        Me.tab_log.ResumeLayout(False)
        Me.tab_log.PerformLayout()
        Me.GroupBox4.ResumeLayout(False)
        Me.GroupBox5.ResumeLayout(False)
        Me.GroupBox5.PerformLayout()
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
    Friend WithEvents CheckBox1 As System.Windows.Forms.CheckBox
    Friend WithEvents Button1 As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox2 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox3 As System.Windows.Forms.CheckBox
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox4 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox5 As System.Windows.Forms.CheckBox
    Friend WithEvents ComboBox1 As System.Windows.Forms.ComboBox
    Friend WithEvents CheckBox6 As System.Windows.Forms.CheckBox
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
End Class
