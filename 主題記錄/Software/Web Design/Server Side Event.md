[本文來源](https://blackbing.medium.com/%E6%B7%BA%E8%AB%87-server-sent-events-9c81ef21ca8e)

前陣子公司專案有個需求需要由 server 通知 client 訊息，因此需要透過 [Server-Sent Events](https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events) 來對 client 發送訊息，前端需要處理接收的部分。因此稍微研究了一下這個標準。

![](https://miro.medium.com/v2/resize:fit:700/1*rdrM4vBJfg0amnkeNtOrfw.jpeg)

Photo by [Sammyayot254](https://unsplash.com/@superadmins?utm_source=unsplash&utm_medium=referral&utm_content=creditCopyText) on [Unsplash](https://unsplash.com/photos/vIQDv6tUHYk?utm_source=unsplash&utm_medium=referral&utm_content=creditCopyText)

# 與 Server 互動

一般的 get/post request 都是基於 HTTP 的標準根據請求來回覆回應的，一旦回應完成連線就會中斷。但如果有些更複雜的需求需要讓後端發送訊息給前端，比較好的方式就是建立 websocket，讓 request 保持連線。不過有時候為了一個需求要建立 websocket 會有點麻煩。因此 Server-Sent Events 就是為了這種需求而誕生的。

# Server-Sent Events

以下簡稱 SSE。若說 websocket 是雙向溝通的話，SSE 其實是單向溝通的，也就是說，一但連線建立之後，就只能接收 Server 端來的訊息。實際的使用情境例如：

- 上傳超大檔案需要通知 client 端上傳處理進度。
- 轉檔需要通知 client 端處理進度。
- 非同步的商業邏輯處理需要通知 client 端狀況。

# 從 Server 傳送訊息

要從 Server 傳送訊息也蠻簡單的，只要在 response 宣告 header 。

Content-Type: "text/event-stream"

接著在需要回應時提供 response 就可以讓 client 端收到正確訊息。具體的範例可以參考 [MDN 的 Using server-sent events](https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events#sending_events_from_the_server) 。

![](https://miro.medium.com/v2/resize:fit:700/1*YTm0oCQujZD3yCTdysECfA.png)

主流瀏覽器都能處理 SSE

# 前端接收訊息

而前端則是透過 [EventSource](https://developer.mozilla.org/en-US/docs/Web/API/EventSource) API 來處理，具體範例也很簡單：

const sse = new EventSource('/api/v1/sse');

sse.addEventListener("notice", (e) => {
  console.log(e.data)
});
sse.addEventListener("update", (e) => {
  console.log(e.data)
})

sse.addEventListener("message", (e) => {
  console.log(e.data)
});

建立好連線之後就可以接收 SSE 的訊息做後續的處理。

# 限制

SSE 的限制是

1. 有限的連線數。
2. 只能處理基本 text 的訊息，
3. 不能自訂 custom header ，例如要傳 Authorization 就不行。這個問題其實蠻大的，因為通常會需要做這種需求都會需要驗證使用者登入狀態，用 header 來處理 auth。

# polyfill

不過還好當初 SSE 在瀏覽器功能尚未完全支援的年代有人開發了 [EventSource polyfill](https://github.com/Yaffle/EventSource)。並且添加了 custom header 的功能，因此大部分的需求也還是可以解決。

# 問題

好其實本文重點現在才要開始。一般來說， polyfill 應該是根據 w3c 的標準，來支援尚未實作的瀏覽器的解決方案。而 EventSource 不支援 custom header 但 polyfill 有實作，這就讓我感到好奇了。

原來早在 2016 年就有人提出這個問題，[Setting headers for EventSource #2177](https://github.com/whatwg/html/issues/2177)，並且也有蠻多討論的，後來有 EventSource 的團隊成員跳出來[回應](https://github.com/whatwg/html/issues/2177#issuecomment-332071504)：

> I work on the team that maintains EventSource for Chrome. EventSource is not being actively developed. We feel resources are better spent in filling the gaps in the more generally-useful facilities provided by fetch().
>
> To give some idea of scale, fetch() body streaming is already used on ten times as many pages as EventSource in Chrome.
>
> I acknowledge that the resumption facilities provided by EventSource are very convenient. However, you can already achieve the same things, with more control, in browsers that support fetch() streaming. I expect that every everfresh browser will support fetch() streaming before any browser supports setting headers for EventSource.
>
> To be clear, I don’t oppose improvements to EventSource. [@danielwegener](https://github.com/danielwegener)’s arguments in [#2177 (comment)](https://github.com/whatwg/html/issues/2177#issuecomment-278254601) are very persuasive. However, my team cannot justify spending any resources on it.
>
> In summary: possible? yes. Likely to happen? no.

説 EventSource 其實沒有更多的開發資源了，而且 fetch 做得更好， fetch streaming 也已經被 chrome 實作了，而且可以接受的 request 量比 EventSource 高十倍以上。雖然 EventSource 如果可以直接支援 custom header 會更方便，但沒有道理要為 fetch 已經可以達到同樣的目的要再增加這個功能。

但因為當時除了 Chrome 其他瀏覽器尚未實作 fetch stream，因此還是激起許多討論，後來有人也發了一個[解決方案](https://github.com/whatwg/html/issues/2177#issuecomment-314178166)，最後被納進 [EventSource polyfill](https://github.com/Yaffle/EventSource)。所以大家就一直用著這個 polyfill 來解決 SSE custom header 的問題。

# Fetch Stream

不過隨著技術的演進，fetch API 也已經可以支援 stream data 了，而且 fetch API 就可以支援各種 custom Header 了，也不再有什麼限制，前端實作也不難，搭配 `ReadableStream` 的 pull callback 就可以接收 Server 發送的訊息。

範例：

fetch(url).then(response => {
  const reader = response.body
    .pipeThrough(new TextDecoderStream())
    .getReader();
  const stream = new ReadableStream({
    start() {
      reader.read().then(() => null);
    },
    pull() {
      reader.read().then(({
        value
      }) => console.log(value))
    }
  })
});

主要是用到了 ReadableStream 取得 Stream 的資料。目前支援度也蠻高的(只看主流瀏覽器的話）因此 ReadableStream 應該是現今處理 SSE 更好的做法。

![](https://miro.medium.com/v2/resize:fit:700/1*HNbTA_r4YAECY1qrqHXQGQ.png)

想看更詳細的資料可以參考這篇文章 [Streaming requests with the fetch API](https://developer.chrome.com/articles/fetch-streaming-requests/#previously-on-the-exciting-adventures-of-fetch-streams)。

# 總結

根據上述的結論，SSE 當初目的是為了提供比 websocket 更簡便實作的方法，由 XMLHttpRequest 延伸而來。但因為 fetch API 的支援度更廣，因此交由 fetch API 去處理 stream 的處理。

會有這次的研究主要也是因為 SSE 不支援 custom header，進而想要了解原因，因此找到了更好的方式來處理，雖然同樣是處理 SSE，fetch 卻叫做 fetch stream，因此我一開始找不到相關的討論。深挖了一下才發現有這段背景故事。

同事問我要怎麼取捨到底是否需要用 polyfill，基本上 polyfill 應該是要根據 spec 支援尚未實作的瀏覽器來做使用。以 [event-source-polyfill](https://github.com/Yaffle/EventSource) 來說，他反而是支援了 spec 沒有的功能，因此我會認為這個 polyfill 的實作只是一種 workaround，可以的話找到 native support 的方法才會是更好的解法。

# Example:

附上一個範例，希望對大家有所幫助。

# References:

- [https://github.com/Yaffle/EventSource](https://github.com/Yaffle/EventSource)
- [https://github.com/whatwg/html/issues/2177](https://github.com/whatwg/html/issues/2177)
- [https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events#sending_events_from_the_server](https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events#sending_events_from_the_server)
- [https://developer.mozilla.org/en-US/docs/Web/API/EventSource](https://developer.mozilla.org/en-US/docs/Web/API/EventSource)
- [https://developer.mozilla.org/en-US/docs/Web/API/ReadableStream](https://developer.mozilla.org/en-US/docs/Web/API/ReadableStream)
- [https://web.dev/streams/](https://web.dev/streams/)

[

](https://medium.com/tag/event-source?source=post_page-----9c81ef21ca8e---------------event_source-----------------)