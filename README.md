Malbolge で HelloWorld するプログラムを探索します。詳しい解説は[ここ](https://qiita.com/reika727/items/1e5f4fb54916c6fed483)を見てってください。

# REQUIREMENT
C++23 から標準ライブラリに導入された関数 (`std::to_underlying`) を使っているので、わりと新しめの g++ か clang++ がいります。

# HOW TO USE
`make test` を叩くだけです。

```console
$ make test
./malbolge-hello.out
GENERATION #1
        GENERATION SIZE: 1
        BEST RESULT    :
        BEST SCORE     : 0

# （中略）

GENERATION #52
        GENERATION SIZE: 10000
        BEST RESULT    : Hello WorlD
        BEST SCORE     : 59

        FINAL RESULT   : Hello WorlD
        FINAL SCORE    : 59
        CODE           : (=<`#9]76ZY32V6/S3,Pq)M'&Jk#Gh~D1#"!~}|{z(Kw%utsVqpihml>jibgJedFFaDY^Wi
```

また、`__tests__` ディレクトリには実装の検証用に作成したインタプリタが入っています。上の例で出力されたコードが本当に動くか確かめてみましょう。

```console
$ cat hello.tmp.mb
(=<`#9]76ZY32V6/S3,Pq)M'&Jk#Gh~D1#"!~}|{z(Kw%utsVqpihml>jibgJedFFaDY^Wi
$ ./malbolge.out hello.tmp.mb
Hello WorlD
```

無事動いてます。

# LICENSE
Malbolge はパブリックドメインです。それに倣い、私もこのリポジトリで公開しているコードに関しては著作権を放棄します。
