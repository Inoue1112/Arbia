[※常に使う行の最後の列まで書き込むこと]
[※整数でお願いします(型は変えられるので連絡ください)]

[壁]
[0:壁無し]
[1:通常壁]

[床]
[0:床無し]
[1:通常床]
[2:半分床左側]
[3:半分床右側]
[4:ペンデュラム床]


[敵]
[ 0:]出さない

[ 1:1人 横]
[ 2:1人 縦]
[ 3:1人 左回転四角]
[ 4:1人 右回転四角]

[ 5:2人 横並び]
[ 6:2人 横互い違い]
[ 7:2人 縦並び]
[ 8:2人 縦互い違い]
[ 9:2人 左回転四角]
[10:2人 右回転四角]

[11:1人 ドアでやられるヤツ]


[○床、縦移動の床]
[0:出さない]
[1以上:動く床を出すのと,]
[ｘ座標をかねている]
[( 入れた数値から-5される｛-5はステージの横幅の半分｝)]
[つまりステージの左端からどれだけずれているか?の数値である]
[最初に配置されるものは大きくなります]
[マイナスの値を入れるとタイミングが反対になります]

[□床、横移動の床:使う場合は床を０番に]
[0:何もないよ]
[1,2:左側に出る]
[3,4:右側に出る]
[初期化時、奇数は東へ、偶数は西へ行く]

[チェックポイント(門):使う場合は床を1番に]
[0:出さない]
[1:ドア設置]

[ペンデュラム:床を1以上にするとペンデュラム床に書き換える、0は床無し]
[0:出さない]
[1:ペンデュラム設置]
[奇数と偶数で最初に振れる方向が逆になる]
[数字を大きくすると、振れはじめのタイミングが遅くなる]

[槍床:使う場合は床を1番に]
[0:出さない]
[1:槍床設置]

[槍壁:使う場合は壁を1番に]
[0:出さない]
[1:槍壁設置]
[2:槍壁設置(タイミングが[1]と逆)]

[落とし穴蓋]
[0:出さない]
[1:落とし穴蓋設置]

[ゴール]
[0:出さない]
[1:出す]