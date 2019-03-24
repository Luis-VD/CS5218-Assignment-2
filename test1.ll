; ModuleID = 'test1.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %sink = alloca i32, align 4
  %source = alloca i32, align 4
  store i32 0, i32* %1
  %2 = call i32 @getchar()
  store i32 %2, i32* %source, align 4
  %3 = load i32* %source, align 4
  store i32 %3, i32* %b, align 4
  %4 = load i32* %a, align 4
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %0
  br label %9

; <label>:7                                       ; preds = %0
  %8 = load i32* %b, align 4
  store i32 %8, i32* %c, align 4
  br label %9

; <label>:9                                       ; preds = %7, %6
  %10 = load i32* %c, align 4
  store i32 %10, i32* %sink, align 4
  %11 = load i32* %1
  ret i32 %11
}

declare i32 @getchar() #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.2-3ubuntu1 (tags/RELEASE_352/final) (based on LLVM 3.5.2)"}
