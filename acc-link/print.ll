; ModuleID = 'print'
source_filename = "print.ll"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr %0, ...)

define void @println() {
println_entry:
  %1 = call i32 (ptr, ...) @printf(ptr @4)
  ret void
}

define void @printi(i32 %0) {
println_entry:
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 %0)
  ret void
}

define void @printiln(i32 %0) {
println_entry:
  %1 = call i32 (ptr, ...) @printf(ptr @1, i32 %0)
  ret void
}

define void @prints(ptr %0) {
println_entry:
  %1 = call i32 (ptr, ...) @printf(ptr @2, ptr %0)
  ret void
}

define void @printsln(ptr %0) {
println_entry:
  %1 = call i32 (ptr, ...) @printf(ptr @3, ptr %0)
  ret void
}
