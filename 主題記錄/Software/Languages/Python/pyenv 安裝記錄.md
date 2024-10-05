
因爲需要在不同 python 版本之間切換，所以找來 pyenv，但是在
`pyenv install 3.6.0` 的時候遇到 segment fau


	if test "xupgrade" != "xno"  ; then \
	        case upgrade in \
	                upgrade) ensurepip="--upgrade" ;; \
	                install|*) ensurepip="" ;; \
	        esac; \
	         ./python -E -m ensurepip \
	                $ensurepip --root=/ ; \
	fi
	Segmentation fault (core dumped)


如果安裝 3.7.9 可以成功，上網查詢後發現可能是 3.6.0 在 build  的時候需要比較舊版的 glibc:[參考資料](https://github.com/pyenv/pyenv/issues/2359)，解決辦法是改用 compiler:

`CC=clang pyenv install 3.6.0`



#

