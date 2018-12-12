# repository My_SAAS
## branch onChip
### 片上系统

1、更新文件夹或者文件的名称
git mv -f oldfolder newfolder
git add -u newfolder (-u选项会更新已经追踪的文件和文件夹)
git commit -m "changed the foldername whaddup"

2、git删除远程仓库的文件或目录 
git rm -r --cached a/2.txt //删除a目录下的2.txt文件 ，如果是删除a目录：git rm -r --cached a
git commit -m "删除a目录下的2.txt文件" 
git push

3、在网页端创建的分支，在本地需要先更新代码到本地，才可以切换。
更新代码到本地： git pull
git branch 看看分支 
git chechout aaa 切换分支aaa 
git branck aaa 创建aaa分支 
git chechout -b aaa 本地创建 aaa分支，同时切换到aaa分支。只有提交的时候才会在服务端上创建一个分支

git push origin aaa 将新增的分支aaa更新到远端服务器

4、commit message
Git 每次提交代码，都要写 Commit message（提交说明），否则就不允许提交。
$ git commit -m "hello world"
上面代码的-m参数，就是用来指定 commit mesage 的。
如果一行不够，可以只执行git commit，就会跳出文本编辑器，让你写多行。
$ git commit

5、fatal: not a git repository (or any of the parent directories): .git
提示说没有.git这样一个目录，解决办法如下：
git init就可以了！


