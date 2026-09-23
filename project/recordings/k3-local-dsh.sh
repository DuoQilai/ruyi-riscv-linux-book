# K3：Q4 llama-server + dsh headless 调 read_status
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey fengde@192.168.31.195
#$ expect \$
#$ snapshot k3-prompt
grep -E "use_ime2|listening" /tmp/llama-server.log
#$ expect listening
#$ snapshot ime2
export PATH="$HOME/.npm-global/bin:$PATH"
set -a
source ~/.dsh/course.env
set +a
dsh --profile headless --patch ~/course-board/course.patch.yml --patch ~/course-board/model-local.patch.yml "现在热不热？先调用 read_status，再根据结果用一句话回答。"
#$ expect 度
#$ snapshot local-answer
exit
#$ expect \$
