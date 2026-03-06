#!/bin/bash

BINARY="./amazed"
PASS=0
FAIL=0
TOTAL=0

run_test()
{
    local desc="$1"
    local input="$2"
    local expect_fail="$3"

    TOTAL=$((TOTAL + 1))
    output=$(echo "$input" | $BINARY 2>/dev/null)
    exit_code=$?

    if [ "$expect_fail" = "1" ]; then
        if [ $exit_code -eq 84 ]; then
            echo "[OK] $desc"
            PASS=$((PASS + 1))
        else
            echo "[KO] $desc (expected failure, got exit $exit_code)"
            FAIL=$((FAIL + 1))
        fi
    else
        if [ $exit_code -eq 0 ]; then
            echo "[OK] $desc"
            PASS=$((PASS + 1))
        else
            echo "[KO] $desc (expected success, got exit $exit_code)"
            FAIL=$((FAIL + 1))
        fi
    fi
}

check_output()
{
    local desc="$1"
    local input="$2"
    local expected="$3"

    TOTAL=$((TOTAL + 1))
    output=$(echo "$input" | $BINARY 2>/dev/null)

    if echo "$output" | grep -q "$expected"; then
        echo "[OK] $desc"
        PASS=$((PASS + 1))
    else
        echo "[KO] $desc (expected '$expected' in output)"
        FAIL=$((FAIL + 1))
    fi
}

check_moves()
{
    local desc="$1"
    local input="$2"
    local max_moves="$3"

    TOTAL=$((TOTAL + 1))
    output=$(echo "$input" | $BINARY 2>/dev/null)
    moves=$(echo "$output" | grep -c "^P")

    if [ "$moves" -le "$max_moves" ]; then
        echo "[OK] $desc"
        PASS=$((PASS + 1))
    else
        echo "[KO] $desc (too many move lines: $moves > $max_moves)"
        FAIL=$((FAIL + 1))
    fi
}

echo "===== BASICS ====="

run_test "01 - Simple linear path 1 robot" \
"1
##start
0 0 0
##end
1 5 0
0-1" 0

run_test "02 - Simple linear path 3 robots" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" 0

run_test "03 - Simple linear path 10 robots" \
"10
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" 0

run_test "04 - Direct path start to end" \
"1
##start
A 0 0
##end
B 1 0
A-B" 0

run_test "05 - Two paths available" \
"3
2 5 0
##start
0 1 2
##end
1 9 2
3 5 4
0-2
0-3
2-1
3-1
2-3" 0

run_test "06 - Many robots linear" \
"50
##start
s 0 0
a 1 0
b 2 0
c 3 0
##end
e 4 0
s-a
a-b
b-c
c-e" 0

run_test "07 - 1 robot direct" \
"1
##start
1 0 0
##end
2 0 5
1-2" 0

run_test "08 - Room order end before start" \
"2
##end
B 5 0
##start
A 0 0
C 2 0
A-C
C-B" 0

run_test "09 - Multiple intermediate rooms" \
"5
##start
s 0 0
a 1 0
b 2 0
c 3 0
d 4 0
##end
e 5 0
s-a
a-b
b-c
c-d
d-e" 0

run_test "10 - Large robot count" \
"100
##start
s 0 0
m 1 0
##end
e 2 0
s-m
m-e" 0

echo ""
echo "===== PARSING ====="

run_test "11 - Comment at start" \
"#comment
3
##start
0 1 0
##end
1 5 0
2 3 0
0-2
2-1" 1

run_test "12 - Comment after robot count" \
"3
#this is a comment
##start
0 1 0
##end
1 5 0
2 3 0
0-2
2-1" 0

run_test "13 - Comment in rooms" \
"2
##start
0 1 0
#mid comment
1 3 0
##end
2 5 0
0-1
1-2" 0

run_test "14 - Comment in tunnels" \
"2
##start
0 1 0
##end
1 5 0
2 3 0
0-2
#comment
2-1" 0

run_test "15 - Comment at end" \
"1
##start
0 0 0
##end
1 5 0
0-1
#end comment" 0

run_test "16 - Multiple comments" \
"2
#c1
##start
#c2
s 0 0
#c3
##end
e 5 0
m 2 0
#c4
s-m
#c5
m-e" 0

run_test "17 - Room with string name" \
"1
##start
kitchen 0 0
##end
bedroom 5 0
kitchen-bedroom" 0

run_test "18 - Room with numeric string name" \
"1
##start
256 0 0
##end
512 5 0
256-512" 0

run_test "19 - Room named zdfg" \
"1
##start
zdfg 0 0
##end
qwerty 5 0
zdfg-qwerty" 0

run_test "20 - Full map with comments" \
"34
##start
1 23 3
2 16 7
#comment
3 16 3
4 16 5
5 9 3
6 1 0
7 4 8
##end
0 9 5
0-4
0-6
1-3
4-3
5-2
3-5
#another comment
4-2
2-1
7-6
7-2
7-4
6-5" 0

echo ""
echo "===== ROBUSTNESS ====="

run_test "21 - No robot count" \
"##start
0 0 0
##end
1 5 0
0-1" 1

run_test "22 - Negative robot count" \
"-1
##start
0 0 0
##end
1 5 0
0-1" 1

run_test "23 - Zero robots" \
"0
##start
0 0 0
##end
1 5 0
0-1" 1

run_test "24 - No start room" \
"3
A 0 0
##end
B 5 0
A-B" 1

run_test "25 - No end room" \
"3
##start
A 0 0
B 5 0
A-B" 1

run_test "26 - No rooms at all" \
"3
0-1" 1

run_test "27 - Multiple start rooms" \
"1
##start
A 0 0
##start
B 1 0
##end
C 5 0
A-C
B-C" 1

run_test "28 - Multiple end rooms" \
"1
##start
A 0 0
##end
B 5 0
##end
C 6 0
A-B
A-C" 1

run_test "29 - No tunnels" \
"1
##start
A 0 0
B 2 0
##end
C 5 0" 1

run_test "30 - No path between start and end" \
"1
##start
A 0 0
B 2 0
##end
C 5 0
A-B" 1

run_test "31 - Tunnel to nonexistent room" \
"1
##start
A 0 0
##end
B 5 0
A-Z" 1

run_test "32 - Duplicate tunnel" \
"1
##start
A 0 0
##end
B 5 0
A-B
A-B" 1

run_test "33 - Room with dash in name" \
"1
##start
A-B 0 0
##end
C 5 0
A-B-C" 1

run_test "34 - Duplicate room name" \
"1
##start
A 0 0
A 1 0
##end
B 5 0
A-B" 1

run_test "35 - Empty input" \
"" 1

run_test "36 - Only robot count" \
"3" 1

run_test "37 - Robot count not a number" \
"abc
##start
A 0 0
##end
B 5 0
A-B" 1

run_test "38 - Room missing coordinate" \
"1
##start
A 0
##end
B 5 0
A-B" 1

run_test "39 - Start not connected" \
"1
##start
A 0 0
B 2 0
##end
C 5 0
B-C" 1

run_test "40 - End not connected" \
"1
##start
A 0 0
B 2 0
##end
C 5 0
A-B" 1

echo ""
echo "===== OUTPUT FORMAT ====="

check_output "41 - Output contains #number_of_robots" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "#number_of_robots"

check_output "42 - Output contains correct robot count" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "^3$"

check_output "43 - Output contains #rooms" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "#rooms"

check_output "44 - Output contains ##start" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "##start"

check_output "45 - Output contains ##end" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "##end"

check_output "46 - Output contains #tunnels" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "#tunnels"

check_output "47 - Output contains #moves" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "#moves"

check_output "48 - Move format P1-roomname" \
"1
##start
0 0 0
##end
1 5 0
0-1" "P1-1"

check_output "49 - Output contains room name" \
"1
##start
kitchen 0 0
##end
bedroom 5 0
kitchen-bedroom" "bedroom"

check_output "50 - Output contains tunnel" \
"1
##start
A 0 0
##end
B 5 0
A-B" "A-B"

check_output "51 - Comments not in output" \
"2
##start
s 0 0
##end
e 5 0
m 2 0
#comment
s-m
m-e" "^[^#]"

check_output "52 - First move is P1" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" "P1-"

echo ""
echo "===== ALGORITHM ====="

run_test "53 - Diamond shape graph" \
"2
##start
s 0 2
a 1 0
b 1 4
##end
e 2 2
s-a
s-b
a-e
b-e" 0

run_test "54 - Loop on path" \
"3
##start
s 0 0
a 1 0
b 2 0
c 1 1
##end
e 3 0
s-a
a-b
b-e
a-c
c-b" 0

run_test "55 - Loop nearby" \
"2
##start
s 0 0
a 1 0
b 2 0
c 2 1
d 1 1
##end
e 3 0
s-a
a-b
b-e
b-c
c-d
d-a" 0

run_test "56 - Multiple valid paths" \
"4
##start
s 0 0
a 1 0
b 1 1
c 2 0
d 2 1
##end
e 3 0
s-a
s-b
a-c
b-d
c-e
d-e" 0

run_test "57 - Long path vs short path" \
"2
##start
s 0 0
a 1 0
b 2 0
c 3 0
d 4 0
shortcut 1 1
##end
e 5 0
s-a
a-b
b-c
c-d
d-e
s-shortcut
shortcut-e" 0

run_test "58 - Star topology" \
"5
##start
s 0 0
a 1 1
b 1 2
c 1 3
d 1 4
##end
e 2 0
s-a
s-b
s-c
s-d
a-e
b-e
c-e
d-e" 0

run_test "59 - Binary tree shape" \
"4
##start
s 0 0
a 1 0
b 1 1
c 2 0
d 2 1
e 2 2
f 2 3
##end
g 3 0
s-a
s-b
a-c
a-d
b-e
b-f
c-g
d-g
e-g
f-g" 0

run_test "60 - Bottleneck path" \
"10
##start
s 0 0
a 1 0
b 2 0
c 3 0
##end
e 4 0
s-a
a-b
b-c
c-e" 0

echo ""
echo "===== OPTIMIZATION ====="

check_moves "61 - Linear 1 robot optimal (3 moves)" \
"1
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" 3

check_moves "62 - Linear 3 robots optimal (5 moves)" \
"3
##start
0 1 0
##end
1 13 0
2 5 0
3 9 0
0-2
2-3
3-1" 5

check_moves "63 - Diamond 2 robots optimal (2 moves)" \
"2
##start
s 0 2
a 1 0
b 1 4
##end
e 2 2
s-a
s-b
a-e
b-e" 2

check_moves "64 - Direct path 1 robot optimal (1 move)" \
"1
##start
A 0 0
##end
B 1 0
A-B" 1

check_moves "65 - Direct path 5 robots optimal (5 moves)" \
"5
##start
A 0 0
##end
B 1 0
A-B" 5

echo ""
echo "===== DATA STRUCTURE ====="

run_test "66 - Bidirectional tunnels" \
"2
##start
A 0 0
B 1 0
##end
C 2 0
A-B
B-C" 0

run_test "67 - Complex graph many rooms" \
"5
##start
s 0 0
a 1 0
b 1 1
c 2 0
d 2 1
e 3 0
f 3 1
##end
g 4 0
s-a
s-b
a-c
b-d
c-e
d-f
e-g
f-g
c-d" 0

run_test "68 - Cycle in graph" \
"3
##start
s 0 0
a 1 0
b 2 0
c 1 1
##end
e 3 0
s-a
a-b
b-e
a-c
c-b
b-a" 0

run_test "69 - Path through many rooms" \
"1
##start
r0 0 0
r1 1 0
r2 2 0
r3 3 0
r4 4 0
r5 5 0
r6 6 0
r7 7 0
r8 8 0
##end
r9 9 0
r0-r1
r1-r2
r2-r3
r3-r4
r4-r5
r5-r6
r6-r7
r7-r8
r8-r9" 0

run_test "70 - Two disjoint paths" \
"2
##start
s 0 0
a1 1 0
a2 2 0
b1 1 1
b2 2 1
##end
e 3 0
s-a1
a1-a2
a2-e
s-b1
b1-b2
b2-e" 0

echo ""
echo "===== EDGE CASES ====="

run_test "71 - Start equals end (same room)" \
"1
##start
##end
A 0 0" 1

run_test "72 - Room with very long name" \
"1
##start
averylongroomnamethatshouldstillwork 0 0
##end
anotherverylongroomname 5 0
averylongroomnamethatshouldstillwork-anotherverylongroomname" 0

run_test "73 - Large coordinates" \
"1
##start
A 999 999
##end
B 0 0
A-B" 0

run_test "74 - Zero coordinates" \
"1
##start
A 0 0
##end
B 0 0
A-B" 0

run_test "75 - Single robot single tunnel" \
"1
##start
s 0 0
##end
e 1 0
s-e" 0

run_test "76 - Many rooms not on path" \
"1
##start
s 0 0
unused1 5 5
unused2 6 6
unused3 7 7
##end
e 1 0
s-e" 0

run_test "77 - Tunnel self-loop" \
"1
##start
A 0 0
##end
B 5 0
A-A
A-B" 1

run_test "78 - Only whitespace robot count" \
"   
##start
A 0 0
##end
B 5 0
A-B" 1

run_test "79 - Float robot count" \
"1.5
##start
A 0 0
##end
B 5 0
A-B" 1

run_test "80 - Very large robot count" \
"999
##start
s 0 0
##end
e 1 0
s-e" 0

echo ""
echo "===== EXTRA PARSING ====="

run_test "81 - Unknown ## command ignored" \
"1
##start
A 0 0
##unknown_command
##end
B 5 0
A-B" 0

run_test "82 - Comment with special chars" \
"1
##start
A 0 0
##end
B 5 0
# this is a comment with special chars: !@#$%
A-B" 0

run_test "83 - Multiple tunnels from start" \
"3
##start
s 0 0
a 1 0
b 1 1
c 1 2
##end
e 2 0
s-a
s-b
s-c
a-e
b-e
c-e" 0

run_test "84 - Multiple tunnels to end" \
"3
##start
s 0 0
a 1 0
b 1 1
c 1 2
##end
e 2 0
s-a
s-b
s-c
a-e
b-e
c-e" 0

run_test "85 - Rooms defined after tunnels" \
"1
##start
A 0 0
A-B
##end
B 5 0" 1

run_test "86 - Duplicate identical rooms" \
"1
##start
A 0 0
B 2 0
B 2 0
##end
C 5 0
A-B
B-C" 1

run_test "87 - Room with same coords as another" \
"1
##start
A 0 0
B 0 0
##end
C 5 0
A-B
B-C" 1

run_test "88 - Tunnel written as end-start" \
"1
##start
A 0 0
##end
B 5 0
B-A" 0

run_test "89 - Start directly linked to end" \
"5
##start
s 0 0
##end
e 1 0
s-e" 0

run_test "90 - Robot count with leading zero" \
"03
##start
A 0 0
##end
B 5 0
A-B" 1

echo ""
echo "===== MORE ALGORITHM ====="

run_test "91 - Three parallel paths" \
"6
##start
s 0 0
a 1 0
b 1 1
c 1 2
##end
e 2 0
s-a
s-b
s-c
a-e
b-e
c-e" 0

run_test "92 - Alternating path lengths" \
"4
##start
s 0 0
short 1 0
long1 1 1
long2 2 1
##end
e 2 0
s-short
short-e
s-long1
long1-long2
long2-e" 0

run_test "93 - Graph with dead ends" \
"2
##start
s 0 0
dead1 1 2
dead2 2 2
a 1 0
##end
e 2 0
s-a
a-e
s-dead1
dead1-dead2" 0

run_test "94 - Robots more than path length" \
"20
##start
s 0 0
m 1 0
##end
e 2 0
s-m
m-e" 0

run_test "95 - All rooms connected to all" \
"3
##start
s 0 0
a 1 0
b 1 1
##end
e 2 0
s-a
s-b
s-e
a-b
a-e
b-e" 0

run_test "96 - Long single path many robots" \
"5
##start
r0 0 0
r1 1 0
r2 2 0
r3 3 0
r4 4 0
##end
r5 5 0
r0-r1
r1-r2
r2-r3
r3-r4
r4-r5" 0

run_test "97 - Two paths different lengths" \
"3
##start
s 0 0
a 1 0
b 2 0
shortcut 1 1
##end
e 3 0
s-a
a-b
b-e
s-shortcut
shortcut-e" 0

run_test "98 - Connected but no path start-end" \
"1
##start
s 0 0
a 1 0
b 2 0
##end
e 3 0
s-a
a-b
b-a" 1

run_test "99 - Graph with bridge" \
"2
##start
s 0 0
a 1 0
bridge 2 0
b 3 0
##end
e 4 0
s-a
a-bridge
bridge-b
b-e" 0

run_test "100 - Max stress test" \
"200
##start
s 0 0
m1 1 0
m2 2 0
m3 3 0
##end
e 4 0
s-m1
m1-m2
m2-m3
m3-e" 0

echo ""
echo "============================="
echo "Total : $TOTAL"
echo "Passed: $PASS"
echo "Failed: $FAIL"
echo "============================="