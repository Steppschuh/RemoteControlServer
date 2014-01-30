<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class WhitelistWindow
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
        Me.Label1 = New System.Windows.Forms.Label()
        Me.tb_whitelist = New System.Windows.Forms.TextBox()
        Me.btn_apply = New System.Windows.Forms.Button()
        Me.label_count = New System.Windows.Forms.Label()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label1.Location = New System.Drawing.Point(12, 9)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(385, 34)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Add the IP addresses of the devices you trust below. Commands from other devices " & _
    "will be rejected. One IP address per line."
        '
        'tb_whitelist
        '
        Me.tb_whitelist.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.tb_whitelist.BackColor = System.Drawing.SystemColors.Window
        Me.tb_whitelist.Location = New System.Drawing.Point(12, 46)
        Me.tb_whitelist.Multiline = True
        Me.tb_whitelist.Name = "tb_whitelist"
        Me.tb_whitelist.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.tb_whitelist.Size = New System.Drawing.Size(385, 171)
        Me.tb_whitelist.TabIndex = 1
        '
        'btn_apply
        '
        Me.btn_apply.Location = New System.Drawing.Point(252, 226)
        Me.btn_apply.Name = "btn_apply"
        Me.btn_apply.Size = New System.Drawing.Size(145, 23)
        Me.btn_apply.TabIndex = 2
        Me.btn_apply.Text = "Apply settings"
        Me.btn_apply.UseVisualStyleBackColor = True
        '
        'label_count
        '
        Me.label_count.AutoSize = True
        Me.label_count.Location = New System.Drawing.Point(9, 231)
        Me.label_count.Name = "label_count"
        Me.label_count.Size = New System.Drawing.Size(121, 13)
        Me.label_count.TabIndex = 3
        Me.label_count.Text = "1 valid IP address found"
        '
        'WhitelistWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(409, 261)
        Me.Controls.Add(Me.label_count)
        Me.Controls.Add(Me.btn_apply)
        Me.Controls.Add(Me.tb_whitelist)
        Me.Controls.Add(Me.Label1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "WhitelistWindow"
        Me.ShowIcon = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Whitelist"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents tb_whitelist As System.Windows.Forms.TextBox
    Friend WithEvents btn_apply As System.Windows.Forms.Button
    Friend WithEvents label_count As System.Windows.Forms.Label
End Class
