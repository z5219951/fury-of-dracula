#!/bin/sh



while true
do
    /web/cs2521/20T2/ass/ass2/hunt/dryrun/rungame.sh dracula.c hunter.c DraculaView.c DraculaView.h HunterView.c HunterView.h GameView.c GameView.h Queue.c Queue.h Makefile utils.c utils.h >test.txt
    FIND_FILE="./test.txt"
    FIND_STR="disqua"
    # 判断匹配函数，匹配函数不为0，则包含给定字符
    if [ `grep -c "$FIND_STR" $FIND_FILE` -ne '0' ];then
        echo "The File Has disqua!"
        exit 0
    fi
    FIND_STR="Final score: 0"
    if [ `grep -c "$FIND_STR" $FIND_FILE` -ne '0' ];then
        echo "The File Win!"
        exit 0
    fi
    tail -5 test.txt
done
