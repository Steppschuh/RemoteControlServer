Imports System.Windows.Media
Imports System.Drawing
Imports System.Drawing.Imaging
Imports System.Text

Module Converter

    Private Function getEncoder(ByVal format As ImageFormat) As ImageCodecInfo
        Dim codecs As ImageCodecInfo() = ImageCodecInfo.GetImageDecoders()
        Dim codec As ImageCodecInfo
        For Each codec In codecs
            If codec.FormatID = format.Guid Then
                Return codec
            End If
        Next codec
        Return Nothing
    End Function

    Public Function bitmapToIcon(ByVal bitmap As Bitmap) As System.Drawing.Icon
        Dim ptr As IntPtr = bitmap.GetHicon
        Return System.Drawing.Icon.FromHandle(ptr)
    End Function

    Public Function bitmapToSource(ByVal bitmap As Bitmap) As ImageSource
        Dim imageSourceConverter As ImageSourceConverter = New ImageSourceConverter()
        Dim source As ImageSource = imageSourceConverter.ConvertFrom(bitmap)
        Return source
    End Function

    Public Function bitmapToByte(ByVal bitmap As Bitmap, ByVal compression As Integer) As Byte()
        Dim img As Image = bitmap

        Dim jgpEncoder As ImageCodecInfo = getEncoder(ImageFormat.Jpeg)
        Dim myEncoder As System.Drawing.Imaging.Encoder = System.Drawing.Imaging.Encoder.Quality
        Dim myEncoderParameters As New EncoderParameters(1)
        Dim myEncoderParameter As New EncoderParameter(myEncoder, compression)
        myEncoderParameters.Param(0) = myEncoderParameter

        Dim MS As New IO.MemoryStream
        Dim format As ImageFormat = ImageFormat.Jpeg
        img.Save(MS, jgpEncoder, myEncoderParameters)
        MS.Flush()
        Return MS.ToArray
    End Function

    Public Function colorToBitmap(ByVal color As System.Drawing.Color) As Bitmap
        Dim bmp As New Bitmap(10, 10, System.Drawing.Imaging.PixelFormat.Format32bppPArgb)
        bmp.SetResolution(72, 72)
        Using G = Graphics.FromImage(bmp)
            G.Clear(color)
            'G.InterpolationMode = Drawing2D.InterpolationMode.HighQualityBicubic
            'G.SmoothingMode = Drawing2D.SmoothingMode.AntiAlias
            'G.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias
        End Using
        Return bmp
    End Function

    Public Function scaleBitmap(ByRef sourceBitmap As Bitmap, ByVal scale As Single)
        If Not scale = 1 And scale > 0 Then
            Dim destinationGraphics As Graphics
            Dim destinationBitmap As Bitmap

            destinationBitmap = New Bitmap(CInt(sourceBitmap.Width * scale), CInt(sourceBitmap.Height * scale))

            destinationGraphics = Graphics.FromImage(destinationBitmap)
            destinationGraphics.DrawImage(sourceBitmap, 0, 0, destinationBitmap.Width + 1, destinationBitmap.Height + 1)
            sourceBitmap = destinationBitmap
        End If
    End Function

    Public Function resizeBitmap(ByRef sourceBitmap As Bitmap, ByVal newWidth As Integer)
        Dim scale As Single
        scale = newWidth / sourceBitmap.Width
        scaleBitmap(sourceBitmap, scale)
    End Function

    Public Function stringToByte(ByVal value As String) As Byte()
        Return System.Text.Encoding.UTF8.GetBytes(value)
    End Function

    Public Function byteToString(ByVal value As Byte()) As String
        Return byteToString(value, 0)
    End Function

    Public Function byteToString(ByVal value As Byte(), ByVal index As Integer) As String
        Dim chars As Char() = System.Text.Encoding.UTF8.GetChars(value, index, value.Length - index)
        Return New String(chars)
    End Function

    Public Function byteToAsciiChar(ByVal b As Byte) As Char
        Dim barr() As Byte = New Byte() {b}
        'Dim carr() As Char = Encoding.ASCII.GetChars(barr)
        Dim carr() As Char = Encoding.GetEncoding(850).GetChars(barr)

        Return carr(0)
    End Function

    Public Function byteToAsciiNumber(ByVal b As Byte) As Byte
        'Converts a value from 0 - 255 to a value from 0 - 9
        Dim value As Byte = 0
        value = (b * 9) / 255
        Return value
    End Function

    Public Function buildCommandData(ByVal identifier As Byte(), ByVal value As Byte()) As Byte()
        addToByteArray(identifier, value)
        Return identifier
    End Function

    Public Function addToByteArray(ByRef destination As Byte(), ByVal source As Byte())
        Dim startIndex As Integer = destination.Length
        Array.Resize(destination, destination.Length + source.Length)
        source.CopyTo(destination, startIndex)
    End Function

    Public Function getPointDistance(ByVal P1 As System.Windows.Point, ByVal P2 As System.Windows.Point, Optional ByVal Digits As Integer = 2) As Decimal
        Dim d As Decimal
        d = Math.Sqrt((Math.Pow((P1.X - P2.X), 2)) + (Math.Pow((P1.Y - P2.Y), 2)))
        d = Math.Round(d, Digits)
        Return d
    End Function

    Public Function stringToBool(ByVal value As String) As Boolean
        If value.ToLower.Equals("1") Or value.ToLower.Equals("true") Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function boolToString(ByVal value As Boolean) As String
        If value Then
            Return "1"
        Else
            Return "0"
        End If
    End Function

    Public Sub setImageDrawable(ByRef imageControl As System.Windows.Controls.Image, ByVal ressource As String)
        Try
            Dim ressourceUri As String = "pack://application:,,,/Resources/" & ressource
            If Not imageControl.Source.ToString.Equals(ressourceUri) Then
                Dim bitmap As BitmapImage = New BitmapImage(New Uri(ressourceUri))
                imageControl.Source = bitmap
            End If
        Catch ex As Exception
        End Try
    End Sub

End Module
