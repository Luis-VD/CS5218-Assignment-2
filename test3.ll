; ModuleID = 'test3.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %sink = alloca i32, align 4
  %source = alloca i32, align 4
  %N = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %1
  %2 = call i32 @getchar()
  store i32 %2, i32* %source, align 4
  store i32 0, i32* %i, align 4
  br label %3

; <label>:3                                       ; preds = %15, %0
  %4 = load i32* %i, align 4
  %5 = load i32* %N, align 4
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %7, label %18

; <label>:7                                       ; preds = %3
  %8 = load i32* %i, align 4
  %9 = srem i32 %8, 2
  %10 = icmp eq i32 %9, 0
  br i1 %10, label %11, label %13

; <label>:11                                      ; preds = %7
  %12 = load i32* %source, align 4
  store i32 %12, i32* %b, align 4
  br label %15

; <label>:13                                      ; preds = %7
  %14 = load i32* %b, align 4
  store i32 %14, i32* %c, align 4
  br label %15

; <label>:15                                      ; preds = %13, %11
  %16 = load i32* %i, align 4
  %17 = add nsw i32 %16, 1
  store i32 %17, i32* %i, align 4
  br label %3

; <label>:18                                      ; preds = %3
  %19 = load i32* %c, align 4
  store i32 %19, i32* %sink, align 4
  %20 = load i32* %1
  ret i32 %20
}

declare i32 @getchar() #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.2-3ubuntu1 (tags/RELEASE_352/final) (based on LLVM 3.5.2)"}
