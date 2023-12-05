

在 Visual Studio Code（以下簡稱 VS Code）中，你可以通過幾個步驟來設置在調試過程中使用的環境變數。這對於模擬不同的環境（如 Debug 和 Release）以及在調試過程中配置不同的變數非常有用。

以下是在 VS Code 中設置環境變數的步驟：

1. 打開你的項目文件夾，然後在 VS Code 中啟動你的調試配置。
2. 在編輯器上方的工具欄中，點擊「選擇和設置調試配置」（或者按下 `F5` 快速鍵）。
3. 如果你還沒有任何調試配置，VS Code 會提示你創建一個。選擇相關的編程語言和環境（例如 Node.js）。
4. 在生成的 `launch.json` 設定文件中，你會看到一個或多個調試配置。
5. 在你想要設置環境變數的調試配置中，添加一個 `"env"` 屬性。這個屬性是一個物件，包含你想要設置的環境變數鍵值對。
    

例如，在 Node.js 調試配置中，你可以這樣設置環境變數：

```json
{
    "type": "node",
    "request": "launch",
    "name": "Debug Node.js App",
    "program": "${workspaceFolder}/app.js",
    "env": {
        "NODE_ENV": "debug",
        "API_KEY": "your_api_key"
    }
}

```

6. 保存 `launch.json` 文件。
7. 現在，當你使用這個調試配置來啟動調試時，VS Code 將會使用你指定的環境變數。
   

請注意，這只會影響你在 VS Code 中的調試過程。如果你希望在實際運行應用時使用環境變數，你需要在你的操作系統中設定相應的環境變數。

以上就是在 VS Code 中設置環境變數的步驟。這將有助於你在調試過程中模擬不同的環境，以及根據需要配置不同的變數。

以下為個人的 launch.json 範例
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    //
    // "program": "${workspaceFolder}\\socket_server\\app\\app.js"
    // "program": "${workspaceFolder}\\socket_server\\tcp_server.js"
    "version": "0.2.0",
    "configurations": [
        {
            "type": "node",
            "request": "launch",
            "name": "Launch Program",
            "skipFiles": [
                "<node_internals>/**"
            ],
            "program": "${workspaceFolder}\\src\\tcp_server.mjs",
            "env": {
                "NODE_SOLUTION": "Debug",
                "API_KEY": "your_api_key"            
        }
    ]
}
```