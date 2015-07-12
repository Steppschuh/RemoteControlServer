<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class IpWindow
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
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(IpWindow))
        Me.btn_IP_help = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.label_IP_address = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.TimerConnected = New System.Windows.Forms.Timer(Me.components)
        Me.SuspendLayout()
        '
        'btn_IP_help
        '
        Me.btn_IP_help.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btn_IP_help.Location = New System.Drawing.Point(265, 139)
        Me.btn_IP_help.Name = "btn_IP_help"
        Me.btn_IP_help.Size = New System.Drawing.Size(170, 23)
        Me.btn_IP_help.TabIndex = 4
        Me.btn_IP_help.Text = "I need help"
        Me.btn_IP_help.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label1.Location = New System.Drawing.Point(12, 9)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(423, 49)
        Me.Label1.TabIndex = 3
        Me.Label1.Text = "To connect with this PC, add the following IP address as a new Server in the Remo" & _
    "te Control app. Your devices need to be connected to the same network."
        '
        'label_IP_address
        '
        Me.label_IP_address.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.label_IP_address.Font = New System.Drawing.Font("Microsoft Sans Serif", 20.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.label_IP_address.ForeColor = System.Drawing.Color.DeepSkyBlue
        Me.label_IP_address.Location = New System.Drawing.Point(12, 56)
        Me.label_IP_address.Name = "label_IP_address"
        Me.label_IP_address.Size = New System.Drawing.Size(423, 43)
        Me.label_IP_address.TabIndex = 5
        Me.label_IP_address.Text = "Unknown"
        Me.label_IP_address.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Label3
        '
        Me.Label3.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label3.ForeColor = System.Drawing.Color.Gray
        Me.Label3.Location = New System.Drawing.Point(12, 100)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(423, 34)
        Me.Label3.TabIndex = 6
        Me.Label3.Text = "( include the dots )"
        Me.Label3.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'TimerConnected
        '
        Me.TimerConnected.Interval = 1000
        '
        'IpWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.Color.White
        Me.ClientSize = New System.Drawing.Size(447, 174)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.label_IP_address)
        Me.Controls.Add(Me.btn_IP_help)
        Me.Controls.Add(Me.Label1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "IpWindow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Server IP Address"
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents btn_IP_help As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents label_IP_address As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents TimerConnected As System.Windows.Forms.Timer
End Class
