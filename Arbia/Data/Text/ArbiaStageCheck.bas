Attribute VB_Name = "ArbiaStageCheck"
Option Explicit
' Public は標準モジュールでのみ定義できる
 Public Type StageFlg
    壁      As Boolean
    床      As Boolean
    敵      As Integer
    丸床    As Integer
    四角床  As Boolean
    ドア    As Boolean
    ペンデュラム As Integer
    槍床    As Boolean
    槍壁    As Boolean
    落とし穴 As Integer
    ゴール  As Boolean
 End Type
Sub エリアチェック()
Attribute エリアチェック.VB_ProcData.VB_Invoke_Func = "n\n14"

    ' Macro1 Macro
    Const 丸床の移動範囲 As Integer = 15
    Const 丸床の出る位置 As Integer = 4
    
    Const 敵の行動範囲 As Integer = 13
    Const 敵の出る位置 As Integer = 6
    
    Const 落とし穴大きさ As Integer = 12
    Const 床モデルマックス As Integer = 4
    Const 壁モデルマックス As Integer = 1
    Dim かぶり防止 As StageFlg
      
    Dim 行最大 As Integer
    
    Dim 列最大 As Integer

    行最大 = Cells(Rows.Count, 1).End(xlUp).Row
    
    列最大 = Range("a1").End(xlToRight).Column
    
   ' MsgBox "指定された列の一番下端のセルは" & r & "行目です。"
     '背景色無しに
    Worksheets("StageData").Cells.Interior.ColorIndex = xlNone
    
    Range("A1:L10").Cells.Interior.ColorIndex = 16
            
   
    'For文開始
     Dim i As Long
        For i = 11 To 行最大
        Dim j As Long
        For j = 1 To 列最大
            '関数チェック用
            'If Not Cells(i, j).HasFormula Then
            If Cells(i, j) = "" Then
             '空白
              Cells(i, j) = 0
            End If
        Next j
            
           '敵の行動範囲か
           If Cells(i + 敵の出る位置, 3).Value <> 0 Then
                If かぶり防止.敵 = 0 Then
                    かぶり防止.敵 = 敵の行動範囲
                    Else
                    MsgBox ("敵の行動範囲どうしかぶっています" & i & "行目です。")
                End If
           End If
          
          '丸床の移動範囲か
           If Cells(i + 4, 4).Value <> 0 Then
                If かぶり防止.丸床 = 0 Then
                    かぶり防止.丸床 = 丸床の移動範囲
                    Else
                    'MsgBox ("丸床の移動範囲どうしかぶっています" & i & "行目です。")
                    かぶり防止.丸床 = 丸床の移動範囲
               End If
           End If
           
           '四角床か
           If Cells(i, 5).Value > 0 Then
              かぶり防止.四角床 = True
              Else
              かぶり防止.四角床 = False
           End If
           
           'ドアか
           If Cells(i, 6).Value > 0 Then
              かぶり防止.ドア = True
              Else
              かぶり防止.ドア = False
           End If
           
            'ペンデュラムか
           If Cells(i, 7).Value > 0 Then
              かぶり防止.ペンデュラム = Cells(i, 7).Value
              Else
              かぶり防止.ペンデュラム = 0
           End If
           
            '槍床か
           If Cells(i, 8).Value > 0 Then
              かぶり防止.槍床 = True
              Else
              かぶり防止.槍床 = False
           End If
           
           '槍壁か
           If Cells(i, 9).Value > 0 Then
              かぶり防止.槍壁 = True
              Else
              かぶり防止.槍壁 = False
           End If
           
           '落とし穴の範囲か
           If Cells(i, 10).Value > 0 Then
                If かぶり防止.落とし穴 <= 0 Then
                かぶり防止.落とし穴 = 落とし穴大きさ
                Else
                MsgBox ("落とし穴どうしがかぶっています" & i & "行目です。")
                End If
           End If
          
          'ゴールか
           If Cells(i, 11).Value > 0 Then
              かぶり防止.ゴール = True
              Else
              かぶり防止.ゴール = False
           End If
           
           
'かぶりかどうか確認はここで
          If かぶり防止.敵 > 0 Then
            If かぶり防止.丸床 > 0 Then
                MsgBox ("敵と丸床がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.四角床 = True Then
                MsgBox ("敵と四角床がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ドア = True Then
                MsgBox ("敵とドアがかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ペンデュラム = True Then
                MsgBox ("敵とペンデュラムがかぶっています" & i & "行目です。")
            End If
            If かぶり防止.槍床 = True Then
                MsgBox ("敵と槍床がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.槍壁 = True Then
                MsgBox ("敵と槍壁がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.落とし穴 > 0 Then
                MsgBox ("敵と落とし穴がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ゴール = True Then
                MsgBox ("敵とゴールがかぶっています" & i & "行目です。")
            End If
          End If
          If かぶり防止.丸床 > 0 Then
            If かぶり防止.四角床 = True Then
                MsgBox ("丸床と四角床がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ドア = True Then
                MsgBox ("丸床とドアがかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ペンデュラム = True Then
                MsgBox ("丸床とペンデュラムがかぶっています" & i & "行目です。")
            End If
            If かぶり防止.槍床 = True Then
                MsgBox ("丸床と槍床がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.槍壁 = True Then
                MsgBox ("丸床と槍壁がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.落とし穴 > 0 Then
                MsgBox ("丸床と落とし穴がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ゴール = True Then
                MsgBox ("丸床とゴールがかぶっています" & i & "行目です。")
            End If
          End If
          If かぶり防止.四角床 > 0 Then
            If かぶり防止.ドア = True Then
                MsgBox ("四角床とドアがかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ペンデュラム = True Then
                MsgBox ("四角床とペンデュラムがかぶっています" & i & "行目です。")
            End If
            If かぶり防止.槍床 = True Then
                MsgBox ("四角床と槍床がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.槍壁 = True Then
                MsgBox ("四角床と槍壁がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.落とし穴 > 0 Then
                MsgBox ("四角床と落とし穴がかぶっています" & i & "行目です。")
            End If
            If かぶり防止.ゴール = True Then
                MsgBox ("四角床とゴールがかぶっています" & i & "行目です。")
            End If
          End If
         If かぶり防止.ドア = True Then
           If かぶり防止.ペンデュラム = True Then
               MsgBox ("ドアとペンデュラムがかぶっています" & i & "行目です。")
           End If
           If かぶり防止.槍床 = True Then
               MsgBox ("ドアと槍床がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.槍壁 = True Then
               MsgBox ("ドアと槍壁がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.落とし穴 > 0 Then
               MsgBox ("ドアと落とし穴がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.ゴール = True Then
               MsgBox ("ドアとゴールがかぶっています" & i & "行目です。")
           End If
         End If
          
         If かぶり防止.ペンデュラム = True Then
           If かぶり防止.槍床 = True Then
               MsgBox ("ペンデュラムと槍床がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.槍壁 = True Then
               MsgBox ("ペンデュラムと槍壁がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.落とし穴 > 0 Then
               MsgBox ("ペンデュラムと落とし穴がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.ゴール = True Then
               MsgBox ("ペンデュラムとゴールがかぶっています" & i & "行目です。")
           End If
         End If
        
        If かぶり防止.槍床 = True Then
           If かぶり防止.槍壁 = True Then
               MsgBox ("槍床と槍壁がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.落とし穴 > 0 Then
               MsgBox ("槍床と落とし穴がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.ゴール = True Then
               MsgBox ("槍床とゴールがかぶっています" & i & "行目です。")
           End If
         End If
      
        If かぶり防止.槍壁 = True Then
            If かぶり防止.落とし穴 > 0 Then
               MsgBox ("槍壁と落とし穴がかぶっています" & i & "行目です。")
           End If
           If かぶり防止.ゴール = True Then
               MsgBox ("槍壁とゴールがかぶっています" & i & "行目です。")
           End If
         End If
        If かぶり防止.落とし穴 > 0 Then
           If かぶり防止.ゴール = True Then
               MsgBox ("落とし穴とゴールがかぶっています" & i & "行目です。")
           End If
         End If
         
         If 床モデルマックス < Cells(i, 2) Then
            Cells(i, 2) = Cells(i, 2) - 床モデルマックス
         End If
         
         If 壁モデルマックス < Cells(i, 1) Then
            Cells(i, 1) = Cells(i, 1) - 壁モデルマックス
         End If
         
         '壁変更
         Const 壁無し As Integer = 0
         Const 通常壁 As Integer = 1
         If かぶり防止.ドア = True Then
            Cells(i, 1) = 通常壁
         ElseIf 0 < かぶり防止.ペンデュラム Then
            Cells(i, 1) = 壁無し
         ElseIf かぶり防止.槍壁 = True Then
            Cells(i, 1) = 通常壁
         ElseIf 0 < かぶり防止.ゴール Then
            Cells(i, 1) = 通常壁
         ElseIf 0 = Cells(i, 1) Then
            Cells(i, 1) = 通常壁
         End If
         '処理軽減用のモデルかどうか
         '壁の最大数を今の値に加えることで変化
         If 0 = Cells(i - 1, 1) Then
            Cells(i, 1) = Cells(i, 1) + 壁モデルマックス
         End If
         If 0 < かぶり防止.ペンデュラム Then
            Cells(i, 1) = 壁無し
         End If
         
         '床変更
         Const 床無し As Integer = 0
         Const 通常床 As Integer = 1
         Const 半分床左 As Integer = 2
         Const 半分床右 As Integer = 3
         Const 傷床 As Integer = 4
         
         If 0 < かぶり防止.敵 Then
            Cells(i, 2) = 通常床
         ElseIf 0 < かぶり防止.丸床 Then
             Cells(i, 2) = 0
         ElseIf かぶり防止.四角床 = True Then
             Cells(i, 2) = 0
         ElseIf かぶり防止.ドア = True Then
             Cells(i, 2) = 通常床
         ElseIf 0 < かぶり防止.ペンデュラム Then
            If 0 < Cells(i, 2) Then
                Cells(i, 2) = 傷床
            End If
         ElseIf かぶり防止.槍床 = True Then
            Cells(i, 2) = 通常床
         ElseIf 0 < かぶり防止.落とし穴 Then
            Cells(i, 2) = 床無し
         ElseIf 0 < かぶり防止.ゴール Then
            Cells(i, 2) = 床無し
         ElseIf 4 = Cells(i, 2) Then
            Cells(i, 2) = 通常床
         End If
         
         '処理軽減用のモデルかどうか
         '床の最大数を今の値に加えることで変化
         If 0 = Cells(i - 1, 2) Then
            If 0 <> Cells(i, 2) Then
                Cells(i, 2) = Cells(i, 2) + 床モデルマックス
            End If
         End If
         If 半分床左 = Cells(i - 1, 2) Then
            If 半分床左 <> Cells(i, 2) Then
                If 0 <> Cells(i, 2) Then
                    Cells(i, 2) = Cells(i, 2) + 床モデルマックス
                End If
            End If
         End If
         If 半分床右 = Cells(i - 1, 2) Then
            If 半分床右 <> Cells(i, 2) Then
                If 0 <> Cells(i, 2) Then
                    Cells(i, 2) = Cells(i, 2) + 床モデルマックス
                End If
            End If
         End If
         
         '壁か
         If Cells(i, 1).Value > 0 Then
            かぶり防止.壁 = True
         Else
            かぶり防止.壁 = False
         End If
           
         '床か
         If Cells(i, 2).Value > 0 Then
            かぶり防止.床 = True
         Else
            かぶり防止.床 = False
         End If
                      
          '色変えは最後に
         If かぶり防止.壁 = True Then
            Cells(i, 1).Interior.Color = 46
         End If
         
         If かぶり防止.床 = True Then
            Cells(i, 2).Interior.Color = 46
         End If
         
         If 0 < かぶり防止.敵 Then
             Cells(i, 3).Interior.Color = 46
             'Cells(i, 3) = 150
             かぶり防止.敵 = かぶり防止.敵 - 1
         End If
         
         If 0 < かぶり防止.丸床 Then
             Cells(i, 4).Interior.Color = 46
             かぶり防止.丸床 = かぶり防止.丸床 - 1
         End If
         
         If かぶり防止.四角床 Then
             Cells(i, 5).Interior.Color = 46
         End If
         
         If かぶり防止.ドア = True Then
             Cells(i, 6).Interior.Color = 46
         End If
         
         If 0 < かぶり防止.ペンデュラム Then
             Cells(i, 7).Interior.Color = 46
             かぶり防止.ペンデュラム = 0
         End If
         
         If かぶり防止.槍床 = True Then
             Cells(i, 8).Interior.Color = 46
         End If
           
         If かぶり防止.槍壁 = True Then
             Cells(i, 9).Interior.Color = 46
         End If
     
         If 0 < かぶり防止.落とし穴 Then
             Cells(i, 10).Interior.Color = 46
             かぶり防止.落とし穴 = かぶり防止.落とし穴 - 1
         End If
         
         If かぶり防止.ゴール = True Then
             Cells(i, 11).Interior.Color = 46
         End If
    
         
    Next i
       
End Sub
