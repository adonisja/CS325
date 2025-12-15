#bin/bash

git pull
git add .
Timestamp=$(date +%F)
git commit -m "Updated @$Timestamp"
git push
echo "Updated Git @$Timestamp"
