; ModuleID = 'main_module'
source_filename = "main_module"

define i32 @main() {
block0:
  %"$temp0" = alloca i32, align 4
  store i32 1, ptr %"$temp0", align 4
  %0 = load i32, ptr %"$temp0", align 4
  ret i32 %0
}
