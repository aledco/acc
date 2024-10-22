; ModuleID = 'println'
source_filename = "println"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr %0, ...)

define void @println(i32 %0) {
println_entry:
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 %0)
  ret void
}
