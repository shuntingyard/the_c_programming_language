"""Fuzz in the sense of testing random valid dates
"""
import datetime
from random import randrange
import subprocess
from sys import stderr

i = 0
ok = 0
ko = 0
for i in range(1, 50002):
    Y = randrange(1, 2500)
    m = randrange(1, 13)
    d = 0
    match m:
        case 2:
            if m % 4 == 0 and m % 100 != 0 or m % 400 == 0:
                d = randrange(1, 30)
            else:
                d = randrange(1, 29)
        case [1, 3, 5, 7, 8, 10, 12]:
            d = randrange(1, 32)
        case _:
            d = randrange(1, 31)
    # prepare reference output
    iso8601 = f"{Y:4}-{m:02}-{d:02}"
    date = datetime.datetime(Y, m, d)
    weekday_weeknumber = date.strftime("%a %W")
    # run process under test
    cp = subprocess.run(
        ["./dateattr", f"{Y}", f"{m}", f"{d}", "fuzz"], capture_output=True
    )
    if cp.returncode != 0:
        # check rc
        print(f"failed on {iso8601}, {cp.stderr.decode('utf-8')}", file=stderr)
        exit(1)
    else:
        out = cp.stdout.decode("utf-8")
        # assert test result
        try:
            assert weekday_weeknumber == out
            ok += 1
        except AssertionError:
            print(f"{iso8601} assertion failure: lhs '{weekday_weeknumber}' rhs {out}")
            ko += 1
    # report
    if i % 1000 == 0:
        print(f"progress i:{i:5}, ok:{ok:5}, ko:{ko:5}")
# report totals
print(f"summary  i:{i:5}, ok:{ok:5}, ko:{ko:5}")
