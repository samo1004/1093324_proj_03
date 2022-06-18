# 1093324_proj_03
# 1093324_proj_1

## 使用變數 :
  _t0_ : 存n\
  _t1_ : 存最後要輸出的值\
  _t2、t3、t4_ : 存一些拿來比較、運算的值
## 程式碼解說 :
  先把一些輸入輸出時的引導字串設定好
  ```asm
  endl:	.string "\n"  #edit
  Input:  .string "Input a number: \n"
  Output:	.string "The damage: \n"
  ```
  依照題義輸入一個整數n，n的範圍為0~99，將n assign到t0(後續都以t0做運算和比較)，接著將t1設定為0，當作累積傷害量的變數，進入Function
  ```asm
  la a0,Input       #load Input 到 a0
	li a7,4         #syscall mode 4 print string
	ecall           #syscall
	li a7,5         #syscall mode 5 enter and put in a0
	ecall           #syscall
	add t0,zero,a0  #assign the entering interger to t0
	
	add t1,x0,x0	#t1 歸0
	jal ra,F	#第一次call function
  ```
  F為計算傷害量的recursive function，每次進入F都先將當前的n值和return address儲存起來，然後判斷要跳到哪個Label做計算
  ```asm
  F:
	addi sp,sp,-8		#空出兩格 #一格-4
	sw ra,0(sp)		#把return address存起來
	sw t0,4(sp)		#把n存起來
	addi t2,zero,0	#t2拿來比較n
	beq t0,zero,xeq0	#n=0
	addi t2,zero,1	#t2=1
	beq t0,t2,xeq1	#n=1
	addi t2,zero,11	#t2=11
	blt t0,t2,xb1s11	#n>1 n<=10
	addi t2,zero,21	#t2=21
	blt t0,t2,xb10s21	#n>10 n<=20
	bge t0,t2,xb20	#>20
  ```
  各個Label都依照function的條件去做運算，計算完成則跳至Exit，return回main function
  ```asm
  xeq0:	
	addi t1,t1,1		#+1
	j Exit
	
  xeq1:
	addi t1,t1,5		#+5
	j Exit
  xb1s11:
	addi t0,t0,-1		#n-1
	jal ra, F		#跳過去做
	addi t0,t0,-1		#n-2
	jal ra,F		#跳過去做
	j Exit		#跳出
  xb10s21:
	addi t0,t0,-2		#n-2
	jal ra,F
	addi t0,t0,-1		#n-3
	jal ra,F
	j Exit
  xb20:
	add t2,zero,t0	#先存個n
	slli t2,t2,1		#n*2
	add t1,t1,t2		#+n*2
	addi t3,zero,5	#等等除5
	div t0,t0,t3		#n/5
	jal ra,F
	j Exit
  Exit:
	lw ra,0(sp)
	lw t0,4(sp)		#把存進去的讀回來
	addi sp,sp,8
	jalr x0,0(ra)
  ```
  最後回到main function輸出
  ```asm
la a0,Output		#ra要回來的地方
li a7,4			#print string
ecall
mv a0,t1		#t1 存ans
li a7,1			#mode 1  print int
ecall
li a7,10
ecall
  ```
## 範例輸入 :
  5\
  10\
  15\
  20\
  25
## 範例輸出 :
  28\
  309\
  1118\
  4472\
  78

