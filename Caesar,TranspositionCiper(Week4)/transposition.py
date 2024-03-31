# 전치 암호 100번 실행
from nltk.corpus import brown
import random, re, time, itertools

# 단어 목록
wordlist = list(set(re.sub('[^a-z]','',i).strip() for i in brown.words()))
wordlist.remove('')

total_duration = 0
for rep in range(100):
    # 측정 시작
    start = time.time()
    
    # 암호문 생성
    plaintext = ""
    key = [0, 1, 2, 3, 4]
    random.shuffle(key)

    print("키: ", key)
    
    n = 7+random.randrange(0,3)
    for i in range(n):
        plaintext += wordlist[random.randrange(1,len(wordlist))]

    print("평문: ", plaintext)

    # 행 맞춤(z추가)
    while len(plaintext)%5 != 0:
        plaintext += "z"

    before_trans = [[] for i in range(len(plaintext)//5)]
    index = 0
    for i in range(0, len(plaintext), 5):
        for char in plaintext[i:i+5]:
            before_trans[index].append(char)
        index += 1

    # 평문을 저장한 2차원 리스트를 전치
    trans = list(zip(*before_trans))
    
    ciper_list = [[] for i in range(5)]
    for i in range(5):
        ciper_list[key[i]] = trans[i]

    ciper = ''
    for i in range(5):
        ciper += ''.join(ciper_list[i])

    print("암호: ", ciper)
    
    # 복호화
    decode_keys = list(itertools.permutations([0,1,2,3,4]))
    decode_list = [['' for j in range(5)] for i in range(len(decode_keys))]
    decode_before_trans = ['' for i in range(len(decode_keys))]
    for i in range(len(decode_keys)):
        for j in range(5):
            decode_list[i][decode_keys[i][j]] = ciper_list[j]

    decode_trans = [[] for i in range(len(decode_keys))]
    for i in range(len(decode_keys)):
        decode_trans[i] = list(zip(*decode_list[i]))

    decode_plaintexts = ['' for i in range(len(decode_keys))]
    for i in range(len(decode_keys)):
        for j in decode_trans[i]:
            decode_plaintexts[i] += ''.join(j)
    
    print("평문 후보군: ")
    n = min(len(ciper), len(max(wordlist, key=len)))
    for s in decode_plaintexts:
        for i in range(n, 2, -1):
            if s[:i] in wordlist:
                print(s)
                break
    
    # 측정 종료
    end = time.time()
    duration = end-start
    print(f"{rep+1}회: {duration:.5f} sec", "="*50, "\n")
    total_duration += duration

print(f"총 실행 시간: {total_duration:.5f}")
print(f"평균 실행 시간: {total_duration/100:.5f}")
