[出處](https://stackoverflow.com/questions/4911794/git-command-for-making-one-branch-like-another/4912267#4912267)

因爲 merge 遇到 conflict 的時候，只有 _採用本分支_ 的選項 : -s ours, 卻沒有 _採用外來分支_ 的選項, 該怎麼辦？

所以 StackOverflow 有人提出這個做法可以 在衝突時採用外來分支的做法：

```
# 這個範例展示 downstream 要 merge upstream 但是衝突都採用 upstream 的修改
git checkout -b tmp origin/upstream  # 先創立 tmp 指向 upstream
git merge -s ours downstream         # ignore all changes from downstream
git checkout downstream
git merge tmp                        # fast-forward to tmp HEAD
git branch -D tmp                    # deleting tmp
```

