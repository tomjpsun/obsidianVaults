UI (vertical)\nTom
	function
		Login user 權限
			權限 profile 從 DB 取得
				DB API for 使用者權限設定 (user privilege profile)
				DB 欄位
					user name
					user ID
					user profile  defines
						super user
						read only user
						privileges
				user Token ?
					Backend register ?
						Backend API
							Error Handling
			同一個 user 是否允許同時 login 多臺 HMI ?
				Backend support
					Backend API
						Error Handling
		Dash board
			RFID
				productivity
					in time
					out time
					duration
					estimation
				management
					準備區佔位
			需要華新提供資料顯示的部分
				current production
				filter
				search
				schedule arangement
		Report
			需要華新提供
				Report format
				search
				filter
				export to other format
			Backend API
				Error Handling
		Management
			Reader antenna map
			system settings
				Import ? Export ?
				Backend API ?
	realization
		figma
			storyboard
				site map
		node.js
			homepage
				sign-in
					user 權限
						權限 profile 從 DB 取得
						同一個 user 是否允許同時 login 多臺 HMI ?
				dashboard
					產線模組(瀑布式)
						current
							檢視位置(floor/region)
					暫位區模組
						目前存放軸/空區域
				sidemenu
					management(after sign-in)
						user management
						機台分配
						準備區分配
						reader/天線分配
						NTP sync
					產線模組(瀑布式) current
						檢視位置(floor/region)
						檢視剛開始的軸
						檢視快做完的軸
						產線模組(瀑布式)
							current
								檢視位置(floor/region)
								檢視剛開始的軸
								檢視快做完的軸
							history 
								檢視位置(floor/region)
					產線模組(瀑布式) history 
						檢視位置(floor/region)
					report
						report state
							本日生產報表
							本週生產報表
							本月生產報表
