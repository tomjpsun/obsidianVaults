reference: [Django and Nginx](https://uwsgi-docs.readthedocs.io/en/latest/tutorials/Django_and_nginx.html)

先安裝本過程需要的 packages

	sudo apt install libpython3-dev gcc

首先安排目錄環境：

	mkdir web
	cd web
	python3 -m venv ENV
	mkdir dj3

這樣，看起來是：

	web
	├── dj3
	└── ENV

 進入虛擬環境,並且安裝需要的 packages：

	source ENV/bin/activate
	cd dj3
	pip install wheel
	pip install django


WARNING: You are using pip version 21.2.3; however, version 21.3 is available.
You should consider upgrading via the '/home/admin/web/ENV/bin/python3 -m pip install --upgrade pip' command.

那就升級 pip:

	python3 -m pip install --upgrade pip

安裝 uwsgi:

	sudo pacman -Syu cmake gcc python3
	pip install uwsgi

在 dj3 下開始一個 Django 專案 'mysite':

	django-admin startproject mysite
	cd mysite

這裡就是我們的專案目錄，在這裏
Create a file called test.py:

    # test.py
	def application(env, start_response):
		start_response('200 OK', [('Content-Type','text/html')])
		return [b"Hello World"] # python3

所以現在目錄長這樣：

	  dj3
		└── mysite               <== here we are
			├── manage.py
			├── mysite
			│   ├── asgi.py
			│   ├── __init__.py
			│   ├── settings.py
			│   ├── urls.py
			│   └── wsgi.py
			└── test.py

接下來我們的 [site] 直接用 IP address 192.168.88.118 來代換，
使用時，必須依照目前機器的 IP 代換之。

	uwsgi --http :8000 --wsgi-file test.py

然後 browser 瀏覽這個 URL：

	http://[site]:8000

應該可以看到 "Hello World", 表示我們已經接通:

	the web client <-> uWSGI <-> Python

可以按 ^C 直接停掉

## 測試我們的 Django Application

我們建立 project 還沒有同步資料庫，進行下一步之前，我們先把 mysite 與 資料庫同步：

	python manage.py migrate

好！現在把我們的 project 加上 admin：

	python manage.py createsuperuser
	Username (leave blank to use 'admin'): admin
		Email address: tom_sun@mail.davicom.com.tw
		Password: admin
		Password (again): admin
		The password is too similar to the username.
		This password is too short. It must contain at least 8 characters.
		This password is too common.
		Bypass password validation and create user anyway? [y/N]: y
		Superuser created successfully.

內容同步一遍（migrate）:

	python manage.py migrate

然後允許其他網址來瀏覽:

	# edit mysite/settings.py, allow all addresses:
		ALLOWED_HOSTS = ['*']

最後用 wsgi 跑起來，不要誤會，mysite.wsgi 不是檔案，而是 mysite/wsgi.py 這個 python 模組，如果我們專案名稱是 xxx，那就要用 --module xxx.wsgi：

	uwsgi --http :8000 --module mysite.wsgi

瀏覽器瀏覽 URL：

	http://[site]:8000/admin

可以看到 admin 登入畫面, 輸入 'admin' 密碼 'admin' 可以進入。
這表示這條路已經打通：

	the web client <-> uWSGI <-> Django

## Basic nginx

	sudo pacman -S nginx
	sudo systemctl enable nginx
	sudo systemctl start nginx

瀏覽器瀏覽 URL：

	http://[site]

應該可以看到 nginx 的 Welcome 畫面.

## Configure nginx for your site

現在我們準備將 nginx 與 uwsgi 接起來，我們需要將 /etc/nginx/uwsgi_params 拷貝到 project 下:

	cp /etc/nginx/uwsgi_params .

然後在 project 下，也編寫一個 wsgi 的 ini 檔案，讓 wsgi 參數都寫在裡面:

	# mysite_nginx.conf

	# the upstream component nginx needs to connect to
	upstream django {
	    # server unix:///path/to/your/mysite/mysite.sock; # for a file socket
	    server 127.0.0.1:8001; # for a web port socket (we'll use this first)
	}

	# configuration of the server
	server {
	    # the port your site will be served on
	    listen      8000;
	    # the domain name it will serve for
	    server_name 192.168.88.118; # substitute your machine's IP address or FQDN
	    charset     utf-8;

	    # max upload size
	    client_max_body_size 75M;   # adjust to taste

	    # Django media
	    location /media  {
	        alias /home/admin/web/dj3/mysite/media;  # your Django project's media files - amend as required
	    }

	    location /static {
	        alias /home/admin/web/dj3/mysite/static; # your Django project's static files - amend as required
	    }

	    # Finally, send all non-media requests to the Django server.
	    location / {
	        uwsgi_pass  django;
	        include     /home/admin/web/dj3/mysite/uwsgi_params; # the uwsgi_params file you installed
	    }
	}

說明：我們先用 port 8001 來接到 django，以後會改用 linux domain socket.
Project 目錄還需要 create ‘media’ 與 ‘static’ 兩個子目錄，將來素材直接放在這裡給 nginx，不需要透過 django application 多繞一次，所以 location 下放絕對路徑。

	mkdir media
	mkdir static

修改 mysite/settings.py，加上 STATIC_ROOT 路徑：

	import os                                        <== add this line
	STATIC_URL = '/static/'
	STATIC_ROOT = os.path.join(BASE_DIR, "static/")  <== add this line

移動 static 素材：

	python manage.py collectstatic

接著告訴 nginx 採用我們的 uwsgi, nginx 需要有一個集中處，可以存放不同的 web apps：

	sudo mkdir -p /etc/nginx/sites-enabled

然後做一個 link 把我們加入：

	sudo ln -s /home/admin/web/dj3/mysite/mysite_nginx.conf /etc/nginx/sites-enabled/

修改 /etc/nginx/nginx.conf:

	http {
		include mime.types;
		default_type application/octet-stream
		include   	  /etc/nginx/sites-enabled/*.conf;     <== add this line
		...

	    keepalive_timeout  65;
	    types_hash_max_size 4096;                          <== add this line
        ...

restart nginx：

	sudo systemctl restart nginx

測試 test.py:

	uwsgi --socket :8001 --wsgi-file test.py

瀏覽

	http://192.168.88.118:8000

可以看到 "Hello World" 表示這個路徑是通的：

	the web client <-> the web server <-> the socket <-> uWSGI <-> Python

## By unix socket

將 port 8001 改用 unix socket，首先，修改 mysite_nginx.conf:

	upstream django {
		# server unix:///path/to/your/mysite/mysite.sock; # for a file socket
		# server 127.0.0.1:8001; # for a web port socket (we'll use this first)  <== mark this
		server unix:///tmp/mysite.sock;                                          <== add this line
	}

然後

	touch /tmp/mysite.sock

重啟 nginx:

	sudo systemctl restart nginx

nginx 的 user id 是 'http' ，我們要允許 http 使用我們的權限，將 'http' 加入我們的 group：

	sudo usermod -aG admin http

測試一下：

	uwsgi --socket /tmp/mysite.sock --plugin python --wsgi-file test.py --chmod-socket=666

Web client 應該可以看到 "Hello World"

## 系統下使用 uwsgi

我們希望 uwsgi 可以直接在系統下操作，而不是 virtual environment 提供：

	sudo pacman -Syu uwsgi uwsgi-plugin-python
	uwsgi --socket /tmp/mysite.sock --plugin python --wsgi-file test.py --chmod-socket=666

最後，在 project 目錄下編寫 mysite_uwsgi.ini:

	# mysite_uwsgi.ini file
	[uwsgi]
	plugins         = python
	# Django-related settings
	# the base directory (full path)
	chdir           = /home/admin/web/dj3/mysite
	# Django's wsgi file
	module          = mysite.wsgi
	# the virtualenv (full path)
	home            = /home/admin/web/ENV

	# process-related settings
	# master
	master          = true
	# maximum number of worker processes
	processes       = 10
	# the socket (use the full path to be safe
	socket          = /tmp/mysite.sock
	# ... with appropriate permissions - may be needed
	chmod-socket    = 664
	# clear environment on exit
	vacuum          = true

現在直接啟動, 注意，我們現在不是在 Python 的 virtual 環境，所以要加參數 --plugin python:

	uwsgi --plugin python --ini mysite_uwsgi.ini

把 plugin 也寫到 .ini :

	# mysite_uwsgi.ini file
	[uwsgi]
	plugins         = python  <== add this line
	# Django-related settings
	...

就不需要 --plugin 參數了

	uwsgi --ini mysite_uwsgi.ini


browser 瀏覽 http://192.168.88.118:8000/ 就可以看到 Django default page.


## 連接 MariaDB

首先，參考 manjaro_install_notes.txt 安裝 MariaDB.

MariaDB 完成後，修改 mysite/settings.py 如下：

	DATABASES = {
	    'default': {
	        'ENGINE': 'django.db.backends.mysql',
	        'NAME': 'inventory', # DB名稱
	        'USER': 'dbuser', # 使用者帳號
	        'PASSWORD': '123456', # 使用者密碼
	        'HOST': 'localhost',
	        'PORT': '3306',
	        'OPTIONS': {
	            'init_command': "SET sql_mode='STRICT_TRANS_TABLES'",
	        },
	    }
	}

安裝相關 package：

	sudo pacman -S mariadb-clients

	# Under Project Environment
	pip install mysqlclient

最後，下 migrate 指令, 從內建 db.sqlite3  移動到 MariaDB：

	python manage.py migrate

Project 目錄下的 db.sqlite3 可以刪除了.

__ [注意] 先前設定的 admin 帳密資訊都放在資料庫。這裡因為資料庫更換，必須重新再設定一次帳密。 __

##  遇到問題如何 debug？

修改 /etc/nginx/nginx.conf:

	- #error_log error.log
    + error_log /var/log/nginx/error.log
    # 注意：在 Manjaro 下，本來就有 /var/log/nginx
    sudo systemctl restart nginx

這樣，nginx 有問題都會記錄在 /var/log/nginx/error.log 裡面。

## admin 看不到 css 素材?

查看 error.log 發現權限不符的錯誤：

	2021/10/19 11:31:39 [error] 5419#5419: *4 open() "/home/admin/web/dj3/mysite/static/admin/css/responsive.css" failed (13: Pe
	rmission denied), client: 192.168.88.100, server: 192.168.88.118, request: "GET /static/admin/css/responsive.css HTTP/1.1",
	host: "192.168.88.118:8000", referrer: "http://192.168.88.118:8000/admin/login/?next=/admin/"

原本 static css 放在 /home/admin/web/dj3/mysite/static，如果權限有問題，
解決辦法是，放到 /var/www/data/static

Manjaro 並沒有這個路徑，所以需要自己建立:

	# 因為 nginx 是以 'http' 的 ID 執行，讓 nginx 自由存取 /var/www 的內容
	sudo mkdir -p /var/www/data/static
	sudo mkdir -p /var/www/data/media

	# 讓 'admin'(屬於 'http' group) 可以修改內容
	sudo chown http:http -R /var/www
	sudo chmod g+rw /var/www/data -R


修改 mysite/settings.py

    - STATIC_ROOT = os.path.join(BASE_DIR, "static/")
	+ STATIC_ROOT = os.path.join("/var/www/data", "static/")

修改 mysite_nginx.conf:

    # Django media
    location /media  {
        alias /var/www/data/media;
    }

    location /static {
        alias /var/www/data/static;
    }

最後記得 restart nginx service：

	sudo systemctl restart nginx

還有 Django collectstatic

	python manage.py collectstatic

##
