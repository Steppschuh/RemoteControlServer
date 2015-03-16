<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class CustomWindow
    Inherits System.Windows.Forms.Form

    'Das Formular überschreibt den Löschvorgang, um die Komponentenliste zu bereinigen.
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

    'Wird vom Windows Form-Designer benötigt.
    Private components As System.ComponentModel.IContainer

    'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
    'Das Bearbeiten ist mit dem Windows Form-Designer möglich.  
    'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.label_count = New System.Windows.Forms.Label()
        Me.btn_apply = New System.Windows.Forms.Button()
        Me.tb_customActions = New System.Windows.Forms.TextBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.SuspendLayout()
        '
        'label_count
        '
        Me.label_count.AutoSize = True
        Me.label_count.Location = New System.Drawing.Point(9, 231)
        Me.label_count.Name = "label_count"
        Me.label_count.Size = New System.Drawing.Size(0, 13)
        Me.label_count.TabIndex = 7
        '
        'btn_apply
        '
        Me.btn_apply.Location = New System.Drawing.Point(252, 226)
        Me.btn_apply.Name = "btn_apply"
        Me.btn_apply.Size = New System.Drawing.Size(145, 23)
        Me.btn_apply.TabIndex = 6
        Me.btn_apply.Text = "Apply settings"
        Me.btn_apply.UseVisualStyleBackColor = True
        '
        'tb_customActions
        '
        Me.tb_customActions.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.tb_customActions.BackColor = System.Drawing.SystemColors.Window
        Me.tb_customActions.Location = New System.Drawing.Point(12, 46)
        Me.tb_customActions.Multiline = True
        Me.tb_customActions.Name = "tb_customActions"
        Me.tb_customActions.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.tb_customActions.Size = New System.Drawing.Size(385, 171)
        Me.tb_customActions.TabIndex = 5
        '
        'Label1
        '
        Me.Label1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label1.Location = New System.Drawing.Point(12, 9)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(385, 34)
        Me.Label1.TabIndex = 4
        Me.Label1.Text = "Add a path or URL for a custom action below. The custom key C1 will invoke a proc" & _
    "ess with the value of line 1."
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(9, 231)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(0, 13)
        Me.Label2.TabIndex = 8
        '
        'CustomWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(409, 261)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.label_count)
        Me.Controls.Add(Me.btn_apply)
        Me.Controls.Add(Me.tb_customActions)
        Me.Controls.Add(Me.Label1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
        Me.Name = "CustomWindow"
        Me.Text = "Custom actions"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents label_count As System.Windows.Forms.Label
    Friend WithEvents btn_apply As System.Windows.Forms.Button
    Friend WithEvents tb_customActions As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
End Class
