Attribute VB_Name = "CsvOut"
Option Explicit
Sub CSV出力()
Attribute CSV出力.VB_Description = "ファイルをCSV出力"
Attribute CSV出力.VB_ProcData.VB_Invoke_Func = "s\n14"
    Dim csvFilePath As String
    Dim iCount As Long
    Dim jCount As Long
    Dim kCount As Long
    Dim maxRow As Long
    Dim maxCol As Long
    Dim fileNo As Integer
    Dim FileName As String
    
    ' ファイル名の取得（拡張子を除く）
    FileName = Left(ActiveWorkbook.Name, InStrRev(ActiveWorkbook.Name, ".", -1, vbTextCompare) - 1)
    ' CSVファイルパスの作成
    csvFilePath = ActiveWorkbook.Path & "\" & FileName & ".csv"
    ' 最終行
    maxRow = ActiveSheet.Range("A1").End(xlDown).Row
    ' 最終列
    maxCol = ActiveSheet.Range("A1").End(xlToRight).Column
    ' ファイル番号
    fileNo = FreeFile

    ' ファイル開く
    Open csvFilePath For Output As #fileNo

    ' 縦方向ループ（最終行まで）
    For iCount = 1 To maxRow
        ' 横方向ループ（最終列－１まで）
        For jCount = 1 To maxCol - 1
            ' ファイル出力（改行なし）
            Print #fileNo, Cells(iCount, jCount) & ",";

            ' ループカウンタをコピー
            kCount = jCount
        Next jCount
        ' 最終列をファイル出力（改行付き）
        Print #fileNo, Cells(iCount, kCount + 1) & ","
    Next iCount

    ' ファイルを閉じる
    Close #fileNo
'
    ActiveWorkbook.Save
End Sub
