# 시저 암호 100번 실행
from nltk.corpus import brown
import random, re, time

# 단어 목록
wordlist = list(set(re.sub('[^a-z]','',i).strip() for i in brown.words()))
wordlist.remove('')

total_duration = 0
for rep in range(100):
    # 측정 시작
    start = time.time()
    
    # 암호문 생성
    plaintext = ""
    caesar = ""
    shift = random.randrange(1, 25)
    n = 7+random.randrange(0,3)
    for i in range(n):
        plaintext += wordlist[random.randrange(1,len(wordlist))] + " "
    
    for v in plaintext:
        if v == " ":
            continue
        new_value = ord(v)+shift
        if new_value > ord('z'):
            new_value -= 26
        caesar += chr(new_value)
    
    print(n, "개 키워드\n", plaintext)
    print(shift, "shift한 암호문\n", caesar)
    
    # 복호화
    decode = ["" for i in range(26)]
    for i in range(1, 26):
        for v in caesar:
            new_value = ord(v)-i
            if new_value < ord('a'):
                new_value += 26
            decode[i-1] += chr(new_value)
        #print(decode[i-1])
    
    print("평문 후보군")
    n = min(len(caesar), len(max(wordlist, key=len)))
    for s in decode:
        for i in range(n, 1, -1):
            if s[:i] in wordlist:
                print(s)
                break
    
    # 측정 종료
    end = time.time()
    duration = end-start
    print(f"{rep+1}회: {duration:.5f} sec")
    total_duration += duration

print(f"총 실행 시간: {total_duration:.5f}")
print(f"평균 실행 시간: {total_duration/100:.5f}")
