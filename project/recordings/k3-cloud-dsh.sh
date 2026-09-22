# K3：云端 deepseek-flash + dsh headless 调 read_status
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey fengde@192.168.31.195
#$ expect \$
#$ snapshot k3-prompt
export PATH="$HOME/.npm-global/bin:$PATH"
#$ expect \$
set -a
#$ expect \$
source ~/.dsh/course.env
#$ expect \$
set +a
#$ expect \$
dsh --profile headless --patch ~/course-board/course.patch.yml --patch ~/course-board/model-cloud.patch.yml "现在热不热？必须先调用 read_status，再根据工具结果用一句话回答。"
#$ expect 度
#$ snapshot cloud-answer
exit
#$ expect \$
