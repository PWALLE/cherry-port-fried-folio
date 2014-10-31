; ModuleID = 'basic.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-apple-darwin10.0"
	%struct.__sFILE = type <{ i8*, i32, i32, i16, i16, i8, i8, i8, i8, %struct.__sbuf, i32, i8, i8, i8, i8, i8*, i32 (i8*)*, i32 (i8*, i8*, i32)*, i64 (i8*, i64, i32)*, i32 (i8*, i8*, i32)*, %struct.__sbuf, %struct.__sFILEX*, i32, [3 x i8], [1 x i8], %struct.__sbuf, i32, i8, i8, i8, i8, i64 }>
	%struct.__sFILEX = type opaque
	%struct.__sbuf = type <{ i8*, i32, i8, i8, i8, i8 }>
	%struct.class_Any = type <{ %struct.class_Any*, %struct.obj_Any* ()*, %struct.obj_String* (%struct.obj_Any*)*, i32 (%struct.obj_Any*, %struct.obj_Any*)* }>
	%struct.class_ArrayAny = type <{ %struct.class_Any*, %struct.obj_ArrayAny* (i32)*, %struct.obj_String* (%struct.obj_ArrayAny*)*, i32 (%struct.obj_ArrayAny*, %struct.obj_Any*)*, i32 (%struct.obj_ArrayAny*)*, %struct.obj_Any* (%struct.obj_ArrayAny*, i32)*, %struct.obj_Any* (%struct.obj_ArrayAny*, i32, %struct.obj_Any*)* }>
	%struct.class_IO = type <{ %struct.class_Any*, %struct.obj_IO* (%struct.obj_IO*)*, i32 (%struct.obj_IO*, %struct.obj_Any*)*, void (%struct.obj_IO*, %struct.obj_String*)*, %struct.obj_IO* (%struct.obj_IO*, %struct.obj_String*)*, i32 (%struct.obj_IO*, %struct.obj_Any*)*, %struct.obj_IO* (%struct.obj_IO*, %struct.obj_Any*)*, %struct.obj_String* (%struct.obj_IO*)*, %struct.obj_Symbol* (%struct.obj_IO*, %struct.obj_String*)*, %struct.obj_String* (%struct.obj_IO*, %struct.obj_Symbol*)* }>
	%struct.class_String = type <{ %struct.class_Any*, %struct.obj_String* ()*, %struct.obj_String* (%struct.obj_String*)*, i32 (%struct.obj_String*, %struct.obj_String*)*, i32 (%struct.obj_String*)*, %struct.obj_String* (%struct.obj_String*, %struct.obj_String*)*, %struct.obj_String* (%struct.obj_String*, i32, i32)*, i8 (%struct.obj_String*, i32)*, i32 (%struct.obj_String*, %struct.obj_String*)* }>
	%struct.class_Symbol = type <{ %struct.class_Any*, %struct.obj_Symbol* ()*, %struct.obj_String* (%struct.obj_Symbol*)*, i32 (%struct.obj_Symbol*, %struct.obj_Any*)* }>
	%struct.obj_Any = type <{ %struct.class_Any*, i8* }>
	%struct.obj_ArrayAny = type <{ %struct.class_ArrayAny*, i32, i8, i8, i8, i8, %struct.obj_Any** }>
	%struct.obj_IO = type <{ %struct.class_IO* }>
	%struct.obj_String = type <{ %struct.class_String*, i32, i8, i8, i8, i8, i8* }>
	%struct.obj_Symbol = type <{ %struct.class_Symbol*, %struct.obj_String* }>
@__stderrp = external global %struct.__sFILE*		; <%struct.__sFILE**> [#uses=2]
@"\01LC" = internal constant [3 x i8] c"%s\00"		; <[3 x i8]*> [#uses=1]
@"\01LC1" = internal constant [5 x i8] c"null\00"		; <[5 x i8]*> [#uses=1]
@"\01LC2" = internal constant [38 x i8] c"Error: input cannot exceed 1000 chars\00"		; <[38 x i8]*> [#uses=1]

define %struct.obj_Any* @newAny() nounwind {
	%1 = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%anyObj = alloca %struct.obj_Any*, align 8		; <%struct.obj_Any**> [#uses=3]
	%2 = call i8* @malloc(i64 16)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_Any*		; <%struct.obj_Any*> [#uses=1]
	store %struct.obj_Any* %3, %struct.obj_Any** %anyObj
	%4 = load %struct.obj_Any** %anyObj		; <%struct.obj_Any*> [#uses=1]
	%5 = getelementptr %struct.obj_Any* %4, i32 0, i32 0		; <%struct.class_Any**> [#uses=1]
	store %struct.class_Any* null, %struct.class_Any** %5
	%6 = load %struct.obj_Any** %anyObj		; <%struct.obj_Any*> [#uses=1]
	store %struct.obj_Any* %6, %struct.obj_Any** %1
	%7 = load %struct.obj_Any** %1		; <%struct.obj_Any*> [#uses=1]
	ret %struct.obj_Any* %7
}

declare i8* @malloc(i64)

define %struct.obj_String* @method_toString(%struct.obj_Any* %this) nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=1]
	%this.addr = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=1]
	store %struct.obj_Any* %this, %struct.obj_Any** %this.addr
	%2 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %2
}

define i32 @method_equals(%struct.obj_Any* %this, %struct.obj_Any* %x) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%this.addr = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%x.addr = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%ret = alloca i32, align 4		; <i32*> [#uses=3]
	store %struct.obj_Any* %this, %struct.obj_Any** %this.addr
	store %struct.obj_Any* %x, %struct.obj_Any** %x.addr
	%2 = load %struct.obj_Any** %this.addr		; <%struct.obj_Any*> [#uses=1]
	%3 = load %struct.obj_Any** %x.addr		; <%struct.obj_Any*> [#uses=1]
	%4 = icmp eq %struct.obj_Any* %2, %3		; <i1> [#uses=1]
	br i1 %4, label %5, label %6

; <label>:5		; preds = %0
	store i32 1, i32* %ret
	br label %7

; <label>:6		; preds = %0
	store i32 0, i32* %ret
	br label %7

; <label>:7		; preds = %6, %5
	%8 = load i32* %ret		; <i32> [#uses=1]
	store i32 %8, i32* %1
	%9 = load i32* %1		; <i32> [#uses=1]
	ret i32 %9
}

define %struct.obj_ArrayAny* @newArrayAny(i32 %length) nounwind {
	%1 = alloca %struct.obj_ArrayAny*		; <%struct.obj_ArrayAny**> [#uses=2]
	%length.addr = alloca i32		; <i32*> [#uses=2]
	%aAnyObj = alloca %struct.obj_ArrayAny*, align 8		; <%struct.obj_ArrayAny**> [#uses=3]
	store i32 %length, i32* %length.addr
	%2 = call i8* @malloc(i64 24)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_ArrayAny*		; <%struct.obj_ArrayAny*> [#uses=1]
	store %struct.obj_ArrayAny* %3, %struct.obj_ArrayAny** %aAnyObj
	%4 = load i32* %length.addr		; <i32> [#uses=1]
	%5 = load %struct.obj_ArrayAny** %aAnyObj		; <%struct.obj_ArrayAny*> [#uses=1]
	%6 = getelementptr %struct.obj_ArrayAny* %5, i32 0, i32 1		; <i32*> [#uses=1]
	store i32 %4, i32* %6
	%7 = load %struct.obj_ArrayAny** %aAnyObj		; <%struct.obj_ArrayAny*> [#uses=1]
	store %struct.obj_ArrayAny* %7, %struct.obj_ArrayAny** %1
	%8 = load %struct.obj_ArrayAny** %1		; <%struct.obj_ArrayAny*> [#uses=1]
	ret %struct.obj_ArrayAny* %8
}

define i32 @method_length(%struct.obj_ArrayAny* %this) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%this.addr = alloca %struct.obj_ArrayAny*		; <%struct.obj_ArrayAny**> [#uses=2]
	store %struct.obj_ArrayAny* %this, %struct.obj_ArrayAny** %this.addr
	%2 = load %struct.obj_ArrayAny** %this.addr		; <%struct.obj_ArrayAny*> [#uses=1]
	%3 = getelementptr %struct.obj_ArrayAny* %2, i32 0, i32 1		; <i32*> [#uses=1]
	%4 = load i32* %3		; <i32> [#uses=1]
	store i32 %4, i32* %1
	%5 = load i32* %1		; <i32> [#uses=1]
	ret i32 %5
}

define %struct.obj_Any* @method_get(%struct.obj_ArrayAny* %this, i32 %index) nounwind {
	%1 = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%this.addr = alloca %struct.obj_ArrayAny*		; <%struct.obj_ArrayAny**> [#uses=2]
	%index.addr = alloca i32		; <i32*> [#uses=2]
	store %struct.obj_ArrayAny* %this, %struct.obj_ArrayAny** %this.addr
	store i32 %index, i32* %index.addr
	%2 = load i32* %index.addr		; <i32> [#uses=1]
	%3 = load %struct.obj_ArrayAny** %this.addr		; <%struct.obj_ArrayAny*> [#uses=1]
	%4 = getelementptr %struct.obj_ArrayAny* %3, i32 0, i32 6		; <%struct.obj_Any***> [#uses=1]
	%5 = load %struct.obj_Any*** %4		; <%struct.obj_Any**> [#uses=1]
	%6 = sext i32 %2 to i64		; <i64> [#uses=1]
	%7 = getelementptr %struct.obj_Any** %5, i64 %6		; <%struct.obj_Any**> [#uses=1]
	%8 = load %struct.obj_Any** %7		; <%struct.obj_Any*> [#uses=1]
	store %struct.obj_Any* %8, %struct.obj_Any** %1
	%9 = load %struct.obj_Any** %1		; <%struct.obj_Any*> [#uses=1]
	ret %struct.obj_Any* %9
}

define %struct.obj_Any* @method_set(%struct.obj_ArrayAny* %this, i32 %index, %struct.obj_Any* %obj) nounwind {
	%1 = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%this.addr = alloca %struct.obj_ArrayAny*		; <%struct.obj_ArrayAny**> [#uses=4]
	%index.addr = alloca i32		; <i32*> [#uses=3]
	%obj.addr = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%ret = alloca %struct.obj_Any*, align 8		; <%struct.obj_Any**> [#uses=2]
	store %struct.obj_ArrayAny* %this, %struct.obj_ArrayAny** %this.addr
	store i32 %index, i32* %index.addr
	store %struct.obj_Any* %obj, %struct.obj_Any** %obj.addr
	%2 = load %struct.obj_ArrayAny** %this.addr		; <%struct.obj_ArrayAny*> [#uses=1]
	%3 = getelementptr %struct.obj_ArrayAny* %2, i32 0, i32 0		; <%struct.class_ArrayAny**> [#uses=1]
	%4 = load %struct.class_ArrayAny** %3		; <%struct.class_ArrayAny*> [#uses=1]
	%5 = getelementptr %struct.class_ArrayAny* %4, i32 0, i32 5		; <%struct.obj_Any* (%struct.obj_ArrayAny*, i32)**> [#uses=1]
	%6 = load %struct.obj_Any* (%struct.obj_ArrayAny*, i32)** %5		; <%struct.obj_Any* (%struct.obj_ArrayAny*, i32)*> [#uses=1]
	%7 = load %struct.obj_ArrayAny** %this.addr		; <%struct.obj_ArrayAny*> [#uses=1]
	%8 = load i32* %index.addr		; <i32> [#uses=1]
	%9 = call %struct.obj_Any* %6(%struct.obj_ArrayAny* %7, i32 %8)		; <%struct.obj_Any*> [#uses=1]
	store %struct.obj_Any* %9, %struct.obj_Any** %ret
	%10 = load %struct.obj_Any** %obj.addr		; <%struct.obj_Any*> [#uses=1]
	%11 = load i32* %index.addr		; <i32> [#uses=1]
	%12 = load %struct.obj_ArrayAny** %this.addr		; <%struct.obj_ArrayAny*> [#uses=1]
	%13 = getelementptr %struct.obj_ArrayAny* %12, i32 0, i32 6		; <%struct.obj_Any***> [#uses=1]
	%14 = load %struct.obj_Any*** %13		; <%struct.obj_Any**> [#uses=1]
	%15 = sext i32 %11 to i64		; <i64> [#uses=1]
	%16 = getelementptr %struct.obj_Any** %14, i64 %15		; <%struct.obj_Any**> [#uses=1]
	store %struct.obj_Any* %10, %struct.obj_Any** %16
	%17 = load %struct.obj_Any** %ret		; <%struct.obj_Any*> [#uses=1]
	store %struct.obj_Any* %17, %struct.obj_Any** %1
	%18 = load %struct.obj_Any** %1		; <%struct.obj_Any*> [#uses=1]
	ret %struct.obj_Any* %18
}

define %struct.obj_String* @newString() nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%stringObj = alloca %struct.obj_String*, align 8		; <%struct.obj_String**> [#uses=4]
	%2 = call i8* @malloc(i64 24)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_String*		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %3, %struct.obj_String** %stringObj
	%4 = load %struct.obj_String** %stringObj		; <%struct.obj_String*> [#uses=1]
	%5 = getelementptr %struct.obj_String* %4, i32 0, i32 1		; <i32*> [#uses=1]
	store i32 0, i32* %5
	%6 = load %struct.obj_String** %stringObj		; <%struct.obj_String*> [#uses=1]
	%7 = getelementptr %struct.obj_String* %6, i32 0, i32 6		; <i8**> [#uses=1]
	store i8* null, i8** %7
	%8 = load %struct.obj_String** %stringObj		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %8, %struct.obj_String** %1
	%9 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %9
}

define %struct.obj_String* @method_String_toString(%struct.obj_String* %this) nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	%2 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %2, %struct.obj_String** %1
	%3 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %3
}

define i32 @method_String_equals(%struct.obj_String* %this, %struct.obj_String* %arg) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%arg.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%match = alloca i32, align 4		; <i32*> [#uses=3]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	store %struct.obj_String* %arg, %struct.obj_String** %arg.addr
	%2 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%3 = getelementptr %struct.obj_String* %2, i32 0, i32 6		; <i8**> [#uses=1]
	%4 = load i8** %3		; <i8*> [#uses=1]
	%5 = load %struct.obj_String** %arg.addr		; <%struct.obj_String*> [#uses=1]
	%6 = getelementptr %struct.obj_String* %5, i32 0, i32 6		; <i8**> [#uses=1]
	%7 = load i8** %6		; <i8*> [#uses=1]
	%8 = call i32 @strcmp(i8* %4, i8* %7)		; <i32> [#uses=1]
	%9 = icmp ne i32 %8, 0		; <i1> [#uses=1]
	br i1 %9, label %10, label %11

; <label>:10		; preds = %0
	store i32 0, i32* %match
	br label %12

; <label>:11		; preds = %0
	store i32 1, i32* %match
	br label %12

; <label>:12		; preds = %11, %10
	%13 = load i32* %match		; <i32> [#uses=1]
	store i32 %13, i32* %1
	%14 = load i32* %1		; <i32> [#uses=1]
	ret i32 %14
}

declare i32 @strcmp(i8*, i8*)

define i32 @method_String_length(%struct.obj_String* %this) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	%2 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%3 = getelementptr %struct.obj_String* %2, i32 0, i32 1		; <i32*> [#uses=1]
	%4 = load i32* %3		; <i32> [#uses=1]
	store i32 %4, i32* %1
	%5 = load i32* %1		; <i32> [#uses=1]
	ret i32 %5
}

define %struct.obj_String* @method_concat(%struct.obj_String* %this, %struct.obj_String* %arg) nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=4]
	%arg.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=3]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	store %struct.obj_String* %arg, %struct.obj_String** %arg.addr
	br i1 false, label %2, label %10

; <label>:2		; preds = %0
	%3 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%4 = getelementptr %struct.obj_String* %3, i32 0, i32 6		; <i8**> [#uses=1]
	%5 = load i8** %4		; <i8*> [#uses=1]
	%6 = load %struct.obj_String** %arg.addr		; <%struct.obj_String*> [#uses=1]
	%7 = getelementptr %struct.obj_String* %6, i32 0, i32 6		; <i8**> [#uses=1]
	%8 = load i8** %7		; <i8*> [#uses=1]
	%9 = call i8* @__strcat_chk(i8* %5, i8* %8, i64 -1)		; <i8*> [#uses=1]
	br label %18

; <label>:10		; preds = %0
	%11 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%12 = getelementptr %struct.obj_String* %11, i32 0, i32 6		; <i8**> [#uses=1]
	%13 = load i8** %12		; <i8*> [#uses=1]
	%14 = load %struct.obj_String** %arg.addr		; <%struct.obj_String*> [#uses=1]
	%15 = getelementptr %struct.obj_String* %14, i32 0, i32 6		; <i8**> [#uses=1]
	%16 = load i8** %15		; <i8*> [#uses=1]
	%17 = call i8* @__inline_strcat_chk(i8* %13, i8* %16)		; <i8*> [#uses=1]
	br label %18

; <label>:18		; preds = %10, %2
	%19 = phi i8* [ %9, %2 ], [ %17, %10 ]		; <i8*> [#uses=0]
	%20 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %20, %struct.obj_String** %1
	%21 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %21
}

declare i8* @__strcat_chk(i8*, i8*, i64)

define internal i8* @__inline_strcat_chk(i8* %__dest, i8* %__src) nounwind {
	%1 = alloca i8*		; <i8**> [#uses=2]
	%__dest.addr = alloca i8*		; <i8**> [#uses=2]
	%__src.addr = alloca i8*		; <i8**> [#uses=2]
	store i8* %__dest, i8** %__dest.addr
	store i8* %__src, i8** %__src.addr
	%2 = load i8** %__dest.addr		; <i8*> [#uses=1]
	%3 = load i8** %__src.addr		; <i8*> [#uses=1]
	%4 = call i8* @__strcat_chk(i8* %2, i8* %3, i64 -1)		; <i8*> [#uses=1]
	store i8* %4, i8** %1
	%5 = load i8** %1		; <i8*> [#uses=1]
	ret i8* %5
}

define %struct.obj_String* @method_substring(%struct.obj_String* %this, i32 %start, i32 %end) nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=3]
	%start.addr = alloca i32		; <i32*> [#uses=4]
	%end.addr = alloca i32		; <i32*> [#uses=4]
	%sub = alloca %struct.obj_String*, align 8		; <%struct.obj_String**> [#uses=5]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	store i32 %start, i32* %start.addr
	store i32 %end, i32* %end.addr
	%2 = call i8* @malloc(i64 24)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_String*		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %3, %struct.obj_String** %sub
	%4 = load i32* %start.addr		; <i32> [#uses=1]
	%5 = load i32* %end.addr		; <i32> [#uses=1]
	%6 = sub i32 %4, %5		; <i32> [#uses=1]
	%7 = load %struct.obj_String** %sub		; <%struct.obj_String*> [#uses=1]
	%8 = getelementptr %struct.obj_String* %7, i32 0, i32 1		; <i32*> [#uses=1]
	store i32 %6, i32* %8
	br i1 false, label %9, label %22

; <label>:9		; preds = %0
	%10 = load %struct.obj_String** %sub		; <%struct.obj_String*> [#uses=1]
	%11 = getelementptr %struct.obj_String* %10, i32 0, i32 6		; <i8**> [#uses=1]
	%12 = load i8** %11		; <i8*> [#uses=1]
	%13 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%14 = getelementptr %struct.obj_String* %13, i32 0, i32 6		; <i8**> [#uses=1]
	%15 = load i8** %14		; <i8*> [#uses=1]
	%16 = load i32* %start.addr		; <i32> [#uses=1]
	%17 = sext i32 %16 to i64		; <i64> [#uses=1]
	%18 = getelementptr i8* %15, i64 %17		; <i8*> [#uses=1]
	%19 = load i32* %end.addr		; <i32> [#uses=1]
	%20 = sext i32 %19 to i64		; <i64> [#uses=1]
	%21 = call i8* @__strncpy_chk(i8* %12, i8* %18, i64 %20, i64 -1)		; <i8*> [#uses=1]
	br label %35

; <label>:22		; preds = %0
	%23 = load %struct.obj_String** %sub		; <%struct.obj_String*> [#uses=1]
	%24 = getelementptr %struct.obj_String* %23, i32 0, i32 6		; <i8**> [#uses=1]
	%25 = load i8** %24		; <i8*> [#uses=1]
	%26 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%27 = getelementptr %struct.obj_String* %26, i32 0, i32 6		; <i8**> [#uses=1]
	%28 = load i8** %27		; <i8*> [#uses=1]
	%29 = load i32* %start.addr		; <i32> [#uses=1]
	%30 = sext i32 %29 to i64		; <i64> [#uses=1]
	%31 = getelementptr i8* %28, i64 %30		; <i8*> [#uses=1]
	%32 = load i32* %end.addr		; <i32> [#uses=1]
	%33 = sext i32 %32 to i64		; <i64> [#uses=1]
	%34 = call i8* @__inline_strncpy_chk(i8* %25, i8* %31, i64 %33)		; <i8*> [#uses=1]
	br label %35

; <label>:35		; preds = %22, %9
	%36 = phi i8* [ %21, %9 ], [ %34, %22 ]		; <i8*> [#uses=0]
	%37 = load %struct.obj_String** %sub		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %37, %struct.obj_String** %1
	%38 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %38
}

declare i8* @__strncpy_chk(i8*, i8*, i64, i64)

define internal i8* @__inline_strncpy_chk(i8* %__dest, i8* %__src, i64 %__len) nounwind {
	%1 = alloca i8*		; <i8**> [#uses=2]
	%__dest.addr = alloca i8*		; <i8**> [#uses=2]
	%__src.addr = alloca i8*		; <i8**> [#uses=2]
	%__len.addr = alloca i64		; <i64*> [#uses=2]
	store i8* %__dest, i8** %__dest.addr
	store i8* %__src, i8** %__src.addr
	store i64 %__len, i64* %__len.addr
	%2 = load i8** %__dest.addr		; <i8*> [#uses=1]
	%3 = load i8** %__src.addr		; <i8*> [#uses=1]
	%4 = load i64* %__len.addr		; <i64> [#uses=1]
	%5 = call i8* @__strncpy_chk(i8* %2, i8* %3, i64 %4, i64 -1)		; <i8*> [#uses=1]
	store i8* %5, i8** %1
	%6 = load i8** %1		; <i8*> [#uses=1]
	ret i8* %6
}

define signext i8 @method_charAt(%struct.obj_String* %this, i32 %index) nounwind {
	%1 = alloca i8		; <i8*> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%index.addr = alloca i32		; <i32*> [#uses=2]
	%ret = alloca i8, align 1		; <i8*> [#uses=2]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	store i32 %index, i32* %index.addr
	%2 = load i32* %index.addr		; <i32> [#uses=1]
	%3 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%4 = getelementptr %struct.obj_String* %3, i32 0, i32 6		; <i8**> [#uses=1]
	%5 = load i8** %4		; <i8*> [#uses=1]
	%6 = sext i32 %2 to i64		; <i64> [#uses=1]
	%7 = getelementptr i8* %5, i64 %6		; <i8*> [#uses=1]
	%8 = load i8* %7		; <i8> [#uses=1]
	store i8 %8, i8* %ret
	%9 = load i8* %ret		; <i8> [#uses=1]
	store i8 %9, i8* %1
	%10 = load i8* %1		; <i8> [#uses=1]
	ret i8 %10
}

define i32 @method_indexOf(%struct.obj_String* %this, %struct.obj_String* %sub) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%this.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%sub.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%ret = alloca i32, align 4		; <i32*> [#uses=2]
	store %struct.obj_String* %this, %struct.obj_String** %this.addr
	store %struct.obj_String* %sub, %struct.obj_String** %sub.addr
	%2 = load %struct.obj_String** %this.addr		; <%struct.obj_String*> [#uses=1]
	%3 = getelementptr %struct.obj_String* %2, i32 0, i32 6		; <i8**> [#uses=1]
	%4 = load i8** %3		; <i8*> [#uses=1]
	%5 = load %struct.obj_String** %sub.addr		; <%struct.obj_String*> [#uses=1]
	%6 = getelementptr %struct.obj_String* %5, i32 0, i32 6		; <i8**> [#uses=1]
	%7 = load i8** %6		; <i8*> [#uses=1]
	%8 = call i64 @strcspn(i8* %4, i8* %7)		; <i64> [#uses=1]
	%9 = trunc i64 %8 to i32		; <i32> [#uses=1]
	store i32 %9, i32* %ret
	%10 = load i32* %ret		; <i32> [#uses=1]
	store i32 %10, i32* %1
	%11 = load i32* %1		; <i32> [#uses=1]
	ret i32 %11
}

declare i64 @strcspn(i8*, i8*)

define %struct.obj_Symbol* @newSymbol() nounwind {
	%1 = alloca %struct.obj_Symbol*		; <%struct.obj_Symbol**> [#uses=2]
	%symObj = alloca %struct.obj_Symbol*, align 8		; <%struct.obj_Symbol**> [#uses=3]
	%str = alloca %struct.obj_String*, align 8		; <%struct.obj_String**> [#uses=4]
	%2 = call i8* @malloc(i64 16)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_Symbol*		; <%struct.obj_Symbol*> [#uses=1]
	store %struct.obj_Symbol* %3, %struct.obj_Symbol** %symObj
	%4 = call i8* @malloc(i64 24)		; <i8*> [#uses=1]
	%5 = bitcast i8* %4 to %struct.obj_String*		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %5, %struct.obj_String** %str
	%6 = load %struct.obj_String** %str		; <%struct.obj_String*> [#uses=1]
	%7 = getelementptr %struct.obj_String* %6, i32 0, i32 0		; <%struct.class_String**> [#uses=1]
	%8 = load %struct.class_String** %7		; <%struct.class_String*> [#uses=1]
	%9 = getelementptr %struct.class_String* %8, i32 0, i32 1		; <%struct.obj_String* ()**> [#uses=1]
	%10 = load %struct.obj_String* ()** %9		; <%struct.obj_String* ()*> [#uses=1]
	%11 = call %struct.obj_String* %10()		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %11, %struct.obj_String** %str
	%12 = load %struct.obj_String** %str		; <%struct.obj_String*> [#uses=1]
	%13 = load %struct.obj_Symbol** %symObj		; <%struct.obj_Symbol*> [#uses=1]
	%14 = getelementptr %struct.obj_Symbol* %13, i32 0, i32 1		; <%struct.obj_String**> [#uses=1]
	store %struct.obj_String* %12, %struct.obj_String** %14
	%15 = load %struct.obj_Symbol** %symObj		; <%struct.obj_Symbol*> [#uses=1]
	store %struct.obj_Symbol* %15, %struct.obj_Symbol** %1
	%16 = load %struct.obj_Symbol** %1		; <%struct.obj_Symbol*> [#uses=1]
	ret %struct.obj_Symbol* %16
}

define %struct.obj_IO* @newIO() nounwind {
	%1 = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=2]
	%ioObj = alloca %struct.obj_IO*, align 8		; <%struct.obj_IO**> [#uses=2]
	%2 = call i8* @malloc(i64 8)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_IO*		; <%struct.obj_IO*> [#uses=1]
	store %struct.obj_IO* %3, %struct.obj_IO** %ioObj
	%4 = load %struct.obj_IO** %ioObj		; <%struct.obj_IO*> [#uses=1]
	store %struct.obj_IO* %4, %struct.obj_IO** %1
	%5 = load %struct.obj_IO** %1		; <%struct.obj_IO*> [#uses=1]
	ret %struct.obj_IO* %5
}

define void @method_abort(%struct.obj_IO* %this, %struct.obj_String* %message) nounwind {
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=1]
	%message.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	store %struct.obj_String* %message, %struct.obj_String** %message.addr
	%1 = load %struct.__sFILE** @__stderrp		; <%struct.__sFILE*> [#uses=1]
	%2 = load %struct.obj_String** %message.addr		; <%struct.obj_String*> [#uses=1]
	%3 = getelementptr %struct.obj_String* %2, i32 0, i32 6		; <i8**> [#uses=1]
	%4 = load i8** %3		; <i8*> [#uses=1]
	%5 = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %1, i8* getelementptr ([3 x i8]* @"\01LC", i32 0, i32 0), i8* %4)		; <i32> [#uses=0]
	call void @exit(i32 1) noreturn
	unreachable
		; No predecessors!
	ret void
}

declare i32 @fprintf(%struct.__sFILE*, i8*, ...)

declare void @exit(i32) noreturn

define %struct.obj_IO* @method_out(%struct.obj_IO* %this, %struct.obj_String* %message) nounwind {
	%1 = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=2]
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=2]
	%message.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	store %struct.obj_String* %message, %struct.obj_String** %message.addr
	%2 = load %struct.obj_String** %message.addr		; <%struct.obj_String*> [#uses=1]
	%3 = getelementptr %struct.obj_String* %2, i32 0, i32 6		; <i8**> [#uses=1]
	%4 = load i8** %3		; <i8*> [#uses=1]
	%5 = call i32 (i8*, ...)* @printf(i8* getelementptr ([3 x i8]* @"\01LC", i32 0, i32 0), i8* %4)		; <i32> [#uses=0]
	%6 = load %struct.obj_IO** %this.addr		; <%struct.obj_IO*> [#uses=1]
	store %struct.obj_IO* %6, %struct.obj_IO** %1
	%7 = load %struct.obj_IO** %1		; <%struct.obj_IO*> [#uses=1]
	ret %struct.obj_IO* %7
}

declare i32 @printf(i8*, ...)

define i32 @method_is_null(%struct.obj_IO* %this, %struct.obj_Any* %arg) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=1]
	%arg.addr = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=2]
	%ret = alloca i32, align 4		; <i32*> [#uses=3]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	store %struct.obj_Any* %arg, %struct.obj_Any** %arg.addr
	%2 = load %struct.obj_Any** %arg.addr		; <%struct.obj_Any*> [#uses=1]
	%3 = icmp eq %struct.obj_Any* %2, null		; <i1> [#uses=1]
	br i1 %3, label %4, label %5

; <label>:4		; preds = %0
	store i32 1, i32* %ret
	br label %6

; <label>:5		; preds = %0
	store i32 0, i32* %ret
	br label %6

; <label>:6		; preds = %5, %4
	%7 = load i32* %ret		; <i32> [#uses=1]
	store i32 %7, i32* %1
	%8 = load i32* %1		; <i32> [#uses=1]
	ret i32 %8
}

define %struct.obj_IO* @method_out_any(%struct.obj_IO* %this, %struct.obj_Any* %arg) nounwind {
	%1 = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=2]
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=4]
	%arg.addr = alloca %struct.obj_Any*		; <%struct.obj_Any**> [#uses=4]
	%msg = alloca %struct.obj_String*, align 8		; <%struct.obj_String**> [#uses=2]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	store %struct.obj_Any* %arg, %struct.obj_Any** %arg.addr
	%2 = load %struct.obj_IO** %this.addr		; <%struct.obj_IO*> [#uses=1]
	%3 = getelementptr %struct.obj_IO* %2, i32 0, i32 0		; <%struct.class_IO**> [#uses=1]
	%4 = load %struct.class_IO** %3		; <%struct.class_IO*> [#uses=1]
	%5 = getelementptr %struct.class_IO* %4, i32 0, i32 5		; <i32 (%struct.obj_IO*, %struct.obj_Any*)**> [#uses=1]
	%6 = load i32 (%struct.obj_IO*, %struct.obj_Any*)** %5		; <i32 (%struct.obj_IO*, %struct.obj_Any*)*> [#uses=1]
	%7 = load %struct.obj_IO** %this.addr		; <%struct.obj_IO*> [#uses=1]
	%8 = load %struct.obj_Any** %arg.addr		; <%struct.obj_Any*> [#uses=1]
	%9 = call i32 %6(%struct.obj_IO* %7, %struct.obj_Any* %8)		; <i32> [#uses=1]
	%10 = icmp eq i32 %9, 1		; <i1> [#uses=1]
	br i1 %10, label %11, label %13

; <label>:11		; preds = %0
	%12 = call i32 (i8*, ...)* @printf(i8* getelementptr ([5 x i8]* @"\01LC1", i32 0, i32 0))		; <i32> [#uses=0]
	br label %25

; <label>:13		; preds = %0
	%14 = load %struct.obj_Any** %arg.addr		; <%struct.obj_Any*> [#uses=1]
	%15 = getelementptr %struct.obj_Any* %14, i32 0, i32 0		; <%struct.class_Any**> [#uses=1]
	%16 = load %struct.class_Any** %15		; <%struct.class_Any*> [#uses=1]
	%17 = getelementptr %struct.class_Any* %16, i32 0, i32 2		; <%struct.obj_String* (%struct.obj_Any*)**> [#uses=1]
	%18 = load %struct.obj_String* (%struct.obj_Any*)** %17		; <%struct.obj_String* (%struct.obj_Any*)*> [#uses=1]
	%19 = load %struct.obj_Any** %arg.addr		; <%struct.obj_Any*> [#uses=1]
	%20 = call %struct.obj_String* %18(%struct.obj_Any* %19)		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %20, %struct.obj_String** %msg
	%21 = load %struct.obj_String** %msg		; <%struct.obj_String*> [#uses=1]
	%22 = getelementptr %struct.obj_String* %21, i32 0, i32 6		; <i8**> [#uses=1]
	%23 = load i8** %22		; <i8*> [#uses=1]
	%24 = call i32 (i8*, ...)* @printf(i8* getelementptr ([3 x i8]* @"\01LC", i32 0, i32 0), i8* %23)		; <i32> [#uses=0]
	br label %25

; <label>:25		; preds = %13, %11
	%26 = load %struct.obj_IO** %this.addr		; <%struct.obj_IO*> [#uses=1]
	store %struct.obj_IO* %26, %struct.obj_IO** %1
	%27 = load %struct.obj_IO** %1		; <%struct.obj_IO*> [#uses=1]
	ret %struct.obj_IO* %27
}

define %struct.obj_String* @method_in(%struct.obj_IO* %this) nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=1]
	%in = alloca %struct.obj_String*, align 8		; <%struct.obj_String**> [#uses=4]
	%numchar = alloca i32, align 4		; <i32*> [#uses=7]
	%c = alloca i8, align 1		; <i8*> [#uses=2]
	%tmp = alloca [1000 x i8], align 1		; <[1000 x i8]*> [#uses=3]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	%2 = call i8* @malloc(i64 24)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_String*		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %3, %struct.obj_String** %in
	store i32 0, i32* %numchar
	br label %4

; <label>:4		; preds = %23, %0
	%5 = call i32 @getchar()		; <i32> [#uses=1]
	%6 = trunc i32 %5 to i8		; <i8> [#uses=2]
	store i8 %6, i8* %c
	%7 = sext i8 %6 to i32		; <i32> [#uses=1]
	%8 = icmp ne i32 %7, 10		; <i1> [#uses=1]
	br i1 %8, label %9, label %24

; <label>:9		; preds = %4
	%10 = load i8* %c		; <i8> [#uses=1]
	%11 = load i32* %numchar		; <i32> [#uses=1]
	%12 = getelementptr [1000 x i8]* %tmp, i32 0, i32 0		; <i8*> [#uses=1]
	%13 = sext i32 %11 to i64		; <i64> [#uses=1]
	%14 = getelementptr i8* %12, i64 %13		; <i8*> [#uses=1]
	store i8 %10, i8* %14
	%15 = load i32* %numchar		; <i32> [#uses=1]
	%16 = add i32 %15, 1		; <i32> [#uses=1]
	store i32 %16, i32* %numchar
	%17 = load i32* %numchar		; <i32> [#uses=1]
	%18 = icmp sge i32 %17, 1000		; <i1> [#uses=1]
	br i1 %18, label %19, label %23

; <label>:19		; preds = %9
	%20 = load %struct.__sFILE** @__stderrp		; <%struct.__sFILE*> [#uses=1]
	%21 = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %20, i8* getelementptr ([38 x i8]* @"\01LC2", i32 0, i32 0))		; <i32> [#uses=0]
	call void @exit(i32 1) noreturn
	unreachable
		; No predecessors!
	br label %23

; <label>:23		; preds = %22, %9
	br label %4

; <label>:24		; preds = %4
	%25 = load i32* %numchar		; <i32> [#uses=1]
	%26 = getelementptr [1000 x i8]* %tmp, i32 0, i32 0		; <i8*> [#uses=1]
	%27 = sext i32 %25 to i64		; <i64> [#uses=1]
	%28 = getelementptr i8* %26, i64 %27		; <i8*> [#uses=1]
	store i8 0, i8* %28
	%29 = getelementptr [1000 x i8]* %tmp, i32 0, i32 0		; <i8*> [#uses=1]
	%30 = load %struct.obj_String** %in		; <%struct.obj_String*> [#uses=1]
	%31 = getelementptr %struct.obj_String* %30, i32 0, i32 6		; <i8**> [#uses=1]
	store i8* %29, i8** %31
	%32 = load i32* %numchar		; <i32> [#uses=1]
	%33 = load %struct.obj_String** %in		; <%struct.obj_String*> [#uses=1]
	%34 = getelementptr %struct.obj_String* %33, i32 0, i32 1		; <i32*> [#uses=1]
	store i32 %32, i32* %34
	%35 = load %struct.obj_String** %in		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %35, %struct.obj_String** %1
	%36 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %36
}

declare i32 @getchar()

define %struct.obj_Symbol* @method_symbol(%struct.obj_IO* %this, %struct.obj_String* %name) nounwind {
	%1 = alloca %struct.obj_Symbol*		; <%struct.obj_Symbol**> [#uses=2]
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=1]
	%name.addr = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%ret = alloca %struct.obj_Symbol*, align 8		; <%struct.obj_Symbol**> [#uses=3]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	store %struct.obj_String* %name, %struct.obj_String** %name.addr
	%2 = call i8* @malloc(i64 16)		; <i8*> [#uses=1]
	%3 = bitcast i8* %2 to %struct.obj_Symbol*		; <%struct.obj_Symbol*> [#uses=1]
	store %struct.obj_Symbol* %3, %struct.obj_Symbol** %ret
	%4 = load %struct.obj_String** %name.addr		; <%struct.obj_String*> [#uses=1]
	%5 = load %struct.obj_Symbol** %ret		; <%struct.obj_Symbol*> [#uses=1]
	%6 = getelementptr %struct.obj_Symbol* %5, i32 0, i32 1		; <%struct.obj_String**> [#uses=1]
	store %struct.obj_String* %4, %struct.obj_String** %6
	%7 = load %struct.obj_Symbol** %ret		; <%struct.obj_Symbol*> [#uses=1]
	store %struct.obj_Symbol* %7, %struct.obj_Symbol** %1
	%8 = load %struct.obj_Symbol** %1		; <%struct.obj_Symbol*> [#uses=1]
	ret %struct.obj_Symbol* %8
}

define %struct.obj_String* @method_symbol_name(%struct.obj_IO* %this, %struct.obj_Symbol* %sym) nounwind {
	%1 = alloca %struct.obj_String*		; <%struct.obj_String**> [#uses=2]
	%this.addr = alloca %struct.obj_IO*		; <%struct.obj_IO**> [#uses=1]
	%sym.addr = alloca %struct.obj_Symbol*		; <%struct.obj_Symbol**> [#uses=2]
	store %struct.obj_IO* %this, %struct.obj_IO** %this.addr
	store %struct.obj_Symbol* %sym, %struct.obj_Symbol** %sym.addr
	%2 = load %struct.obj_Symbol** %sym.addr		; <%struct.obj_Symbol*> [#uses=1]
	%3 = getelementptr %struct.obj_Symbol* %2, i32 0, i32 1		; <%struct.obj_String**> [#uses=1]
	%4 = load %struct.obj_String** %3		; <%struct.obj_String*> [#uses=1]
	store %struct.obj_String* %4, %struct.obj_String** %1
	%5 = load %struct.obj_String** %1		; <%struct.obj_String*> [#uses=1]
	ret %struct.obj_String* %5
}

define i32 @ll_main(i32 %junk) nounwind {
	%1 = alloca i32		; <i32*> [#uses=2]
	%junk.addr = alloca i32		; <i32*> [#uses=1]
	store i32 %junk, i32* %junk.addr
	store i32 0, i32* %1
	%2 = load i32* %1		; <i32> [#uses=1]
	ret i32 %2
}
