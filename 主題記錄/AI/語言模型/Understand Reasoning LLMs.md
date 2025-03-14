本篇是 Sebastian 對於微調 LLM 其中的 Reasoning Model 深入的介紹, 並且也介紹了 DeepSearch 採用的推理方法

https://sebastianraschka.com/blog/2025/understanding-reasoning-llms.html

by Sebastian Raschka

#CoT 

Reasoning Language Models (RLMs): 技術藍圖解密

最近 AI 領域炙手可熱的 Reasoning Language Models (RLMs)，這些模型像是 OpenAI 的 o1、DeepSeek-R1 和阿里巴巴的 QwQ，它們透過強化大型語言模型 (LLMs) 的推理機制，重新定義了 AI 解決問題的能力。![[主題記錄/AI/語言模型/attachments/4317d6486305e4b47943e6b89afeb536_MD5.png]]

▋ 什麼是推理語言模型？

想像一下，普通的 AI 模型就像是記憶力超強的學生，可以背誦大量的知識。但 RLM 模型更厲害，它不只記憶力好，還懂得推理，就像偵探一樣，能從線索中找出答案！![[主題記錄/AI/語言模型/attachments/f3f03be3dd9b45f099da0a8b3dc921f4_MD5.png]]

▋ RLM 的三大要素 (Three Pillars) ![[主題記錄/AI/語言模型/attachments/99edafd6dbb50c52d460c675dca3f249_MD5.png]]

要打造一個聰明的 RLM 模型，需要以下三個要素：

1. 知識庫：大型語言模型 (LLMs)就像一本百科全書，提供 AI 模型豐富的知識。![[主題記錄/AI/語言模型/attachments/a73dbf3c5e19cb03dbd728a43fbe7de5_MD5.png]]

2. 思考力：強化學習 (RL) 讓 AI 模型能像人類一樣，透過不斷嘗試和錯誤，學習解決問題的最佳策略。![[主題記錄/AI/語言模型/attachments/55ae3f928ef4c4705055d53a42b19d7d_MD5.png]]

3. 算力：高效能計算 (HPC) 提供強大的算力，讓 AI 模型可以快速學習和推理。![[主題記錄/AI/語言模型/attachments/c073ff9fd5908c43071d67cbce94b11e_MD5.png]]

▋ RLM 藍圖 (Blueprint) ![[主題記錄/AI/語言模型/attachments/47b3e1bd7c68d72cfdf48be9b550c940_MD5.png]]

這個藍圖就像一個工具箱，裡面包含了構建各種 RLM 所需的要素。它主要分為以下幾個部分：

➤ 推理步驟：就像偵探的每一個腳印，是推理的基本單位。![[主題記錄/AI/語言模型/attachments/864445bba8f2085c703ca40de409ed35_MD5.png]]

➤ 推理結構：就像偵探的思維導圖，將推理步驟組織起來，常見的結構有以下幾種：

- 鏈 (Chain)：線性推理流程，例如 CoT。![[主題記錄/AI/語言模型/attachments/043ac96c4ff8b95ce74f1fad4bd2b524_MD5.png]]

- 樹 (Tree)：階層式分支，例如 ToT。![[主題記錄/AI/語言模型/attachments/eb61c3638e6dcacc4701b77c62d54fff_MD5.png]]

- 圖 (Graph)：任意連接，例如 GoT。![[主題記錄/AI/語言模型/attachments/635541ee925b57559049ff3251a89e4f_MD5.png]]

- 巢狀 (Nesting)：節點本身可以包含子結構。![[主題記錄/AI/語言模型/attachments/e41f3caf1e1d6a19ee02345135765a53_MD5.png]]

➤ 運算元：就像偵探使用的工具，可以改變推理結構，常見的運算元有：

- 生成 (Generate)：新增推理步驟，例如 MCTS 中的 Expand。![[主題記錄/AI/語言模型/attachments/a2e7828e9baa61cc295216502b98aaf3_MD5.png]]

- 精煉 (Refine)：增強現有推理步驟，例如糾正錯誤。![[主題記錄/AI/語言模型/attachments/9dc5cb1c4438c540a4f4bc3dccaefe53_MD5.png]]

- 聚合 (Aggregate)：合併多個推理步驟，例如 GoT。![[主題記錄/AI/語言模型/attachments/c250c7cbd170605cf25d190c5eaf97cd_MD5.png]]

- 修剪 (Prune)：移除不必要的推理步驟。![[主題記錄/AI/語言模型/attachments/85a227ba59f6fd0d2ab36e9932d3cd7d_MD5.png]]

- 重構 (Restructure)：任意結構轉換，例如 Journey Learning。![[主題記錄/AI/語言模型/attachments/01103c752c36dab6450ce8e18047bf77_MD5.png]]

➤ 模型：就像偵探的經驗，可以評估推理路徑的品質，常見的模型有：

- 價值模型 (Value Model)：評估推理路徑的品質。![[主題記錄/AI/語言模型/attachments/a901db2d1a16c156482c08dc742ff63e_MD5.png]]

- 策略模型 (Policy Model)：生成新的推理步驟。![[主題記錄/AI/語言模型/attachments/0059c944a4ecdc1b81f369c2ee114815_MD5.png]]

- 獎勵模型 (Reward Model)：評估單個推理步驟的品質。![[主題記錄/AI/語言模型/attachments/5797d4a7a1b9c86432d68bed10e2eda4_MD5.png]]

➤ 資料訓練：就像偵探學習的過程，需要大量的案例和資料，包含：

- 基於結果的監督 (Outcome-Based Supervision)：只有輸入和輸出，例如數學題的題目和答案。

- 基於過程的監督 (Process-Based Supervision)：包含所有中間步驟，例如 CoT。

- 基於追蹤的監督 (Trace-Based Supervision)：將應用操作序列的詳細資訊包含在推理結構中。

▋ RLM 實測：用 x1 框架打造 AI 大腦 ![[主題記錄/AI/語言模型/attachments/702b1fd3fa8118c29c00017e7b9c422d_MD5.png]]

為了驗證 RLM 的效果，研究人員還開發了一個叫做「x1」的框架，讓大家可以輕鬆打造和實驗自己的 RLM 模型。這個框架就像一個積木，讓你可以自由組合各種推理工具，x1 的主要特點：

- 靈活的推理結構：利用樹狀推理結構和 MCTS 策略。

- 多樣化的運算元：支援多種運算元，例如 Generate、Select 和 Backpropagate。

- 模組化設計：方便使用者自定義和擴展。

- 可擴展性：支援多 GPU 和多節點部署。

▋ RLM 重要發現 (Key Insights)

➤ 兩階段訓練：分階段訓練策略（SFT 和 RL）已被證明在多種情況下有效。這種分階段的方法允許模型首先在第一階段學習推理模式的堅實基礎，然後在第二階段在更複雜、自適應的條件下進行微調。

➤ 熟悉資料：在熟悉的資料分佈上進行訓練可以顯著影響模型的初始效能和後續改進。

➤ 小心提示：單獨依靠提示來鼓勵大型語言模型來評論和評估它們自己的輸出通常會導致不穩定。

▋ RLM 的未來：讓 AI 更加聰明！![[主題記錄/AI/語言模型/attachments/65b6097ad09caf824b6e07129a191cab_MD5.png]]

推理語言模型 (RLM) 的出現，讓 AI 變得更加聰明、可靠，也為我們打開了更多可能性。未來，我們可以期待 RLM 在各個領域大放異彩，例如：

➤ 醫療保健：輔助診斷、提供個人化治療建議。![[主題記錄/AI/語言模型/attachments/30f85e249a2bdb2eeec73c7c6c05ff5a_MD5.png]]

➤ 金融：風險評估、詐欺檢測。![[主題記錄/AI/語言模型/attachments/a901db2d1a16c156482c08dc742ff63e_MD5.png]]

➤ 科學研究：加速科學發現、解決複雜問題。![[主題記錄/AI/語言模型/attachments/7c2223153519816fc4348fe4608a89e6_MD5.png]]

—

▋ Paper
https://arxiv.org/pdf/2501.11223
