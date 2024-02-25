# SAT-Solver
Final Project of Intro to CS
## 摘要
這個project的目標是設計一個 DPLL 的演算法解決 SAT 問題，當使用者輸入含有文字 (literal)、子句(clause)的 CNF (Conjunctive Normal Form)檔案，演算法即能算出是否存在一組變量賦值，使問題為可滿足。
## 程式碼內容
1. DPLL (Davis-Putnam-Logemann-Loveland)
   + 概念

     DPLL是一種基於回溯的演算法，此算法會嘗試每種可能的賦值，直到找到一組為true賦值為止。而DPLL可以利用單位傳播(Unit Propagation)步驟來進行更有效率的猜測，從而減小搜索樹的大小，可視為一種剪枝策略。在求解的過程中，如果我們發現某個子句的真值為真，就刪除這個子句;如果發現某個文字的真值為假，就在子句中刪除這個變量。直到CNF為空，說明所有子句都滿足了，即該CNF是可滿足的;如果發現CNF中有一個空子句沒有被刪除，說明這個子句的所有文字取值都為假，即這個子句無法滿足，此CNF無法滿足。
   * Pseudo Code
  
     ```c++
     
   * Unit Propagation

     如果一個CNF包含一個單位子句L(只有一个文字的子句)，那麼該CNF中的其他子句就可以被L化簡，包括下列兩個步驟:
      1. 如果某個子句中含有L，那麼就把這個子句移除。
      2. 如果某個子句中含有¬ L，則在此子句中把¬ L移除。
      利用Unit Propagation化簡後，CNF可能會再出現其他單位子句，因此我們就可以持續應用Unit Propagation，直到最簡為止。
   * DLIS(Dynamic Largest Individual Sum)

     統計初始時頻次最高的文字，優先從這個文字開始處理，並依出現頻次往下判斷，即可縮短演算法的執行時間。公式如下:

     <img width="210" alt="截圖 2024-02-26 上午12 32 11" src="https://github.com/jeannie068/SAT-Solver/assets/124335771/ba9bb1bd-6d20-4e71-b61e-ac7e40da901f">
3. 程式碼執行
## Completion of this assignment
我完成了實作DPLL演算法解決SAT問題，其中演算法內包含了Unit propagation、 Pure Literal Assign、DLIS等程序，並成功使用工作站輸出與參考檔案一樣的格式及解答。
## 待進步的地方
此次多方查找資料時，發現CDCL(Conflict-Driven Clause Learning)演算法更為快 速，其原理是在DPLL上更進一步延伸，使用DPLL時有時候錯誤早已出現，但其程 序只能回溯一層。CDCL就進一步優化，使用了非時序性回溯，能根據衝突來進行 更高的回溯，結果來看大大提升了SAT的執行效率。以下為CDCL演算法的Pseudo Code(來源:https://blog.csdn.net/qaqwqaqwq/article/details/126020807) ，但可惜我在此次作業中沒有實作出來，因此我認為我的程式還可以再進一步優化成 CDCL演算法。
```C++
CDCL(CNF):
     副本 = CNF // 創建CNF的副本，不更改原CNF
     while true:
         while 副本含有單位子句:
             對副本使用單位傳播;
         if 副本中含有取值為假的子句: // 發現衝突
             if 現在的決策層是0:
                 return false; // 不能滿足
             C = 子句學習(CNF, 副本) // 吸取教訓
             根據C回到一個更早的決策層; // 調整策略
         else:
             if 所有變量都被賦值:
                 return true; // 可滿足
             else: // 進行一次決策（決策就是一次嘗試，令某個文字為真，撞大運）
                 開始一個新的決策層;
                 找到一個未賦值的文字l;
                 副本 = 副本∧{l}
                 // 給l賦值為真
                 // 加入l構成的單位子句，使得副本要滿足就是l要滿足，變相地要求l為真
                 // 對於變量x，若給x賦值為真，就令l = x；若給x賦值為假，就令l = ¬x
```
## Reference
* 想法、觀念參考 :
  https://cse442-17f.github.io/Conflict-Driven-Clause-Learning/
  Clarke, E.M., Henzinger, Th.A., Veith, H., Bloem, R. (Eds.). Handbook of Model Checking. 2018.
* 程式碼實作參考 :
  https://github.com/HJNODM/DPLL-SAT-Solver
* Class Materials
