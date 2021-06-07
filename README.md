# dateseq

_Like `seq`, but for dates._

## Usage

Sequence based on absolute YYYY-MM-DD date:

```
$ dateseq 2021-05-31
2021-05-31
2021-06-01
2021-06-02
2021-06-03
```

Sequence based on relative time difference:

```
$ dateseq -2d
2021-06-01
2021-06-02
```

Relative time includes positive (future) references, and day/week/month/year units:

```
dateseq +1w
2021-06-03
2021-06-04
2021-06-05
2021-06-06
2021-06-07
2021-06-08
2021-06-09
```

Sequence between two dates, including backwards:

```
dateseq 2021-06-03 2021-05-30
2021-06-03
2021-06-02
2021-06-01
2021-05-31
```

## Install

* Compile with `make`.
* Place `dateseq` anywhere included in your shell's `$PATH`.
