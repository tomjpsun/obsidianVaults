---
tags:
  - setField
  - field
  - QWizardPage
  - registerField
---
QWizardPage  提供 registerField()  可以 *註冊*  一個屬性來對應 UI 元件
然後用 setField() / field() 來寫入/讀取 UI 的值。但是文件沒有說明，setField() / field() **必須在 QWizardPage constructor 外面使用**，否則不 work，也沒錯誤訊息。

S.O. 有人討論這個：
https://stackoverflow.com/questions/41040906/qwizardpage-registerfield-for-class-property-not-working

