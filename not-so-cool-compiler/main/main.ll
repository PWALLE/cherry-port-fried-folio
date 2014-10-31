;#################################################################
;###                    auxiliary stuff                        ###
;#################################################################


target triple = "x86_64-apple-darwin10.0"
@str.format = private constant [3 x i8] c"%s\00"
@str.format2 = private constant [3 x i8] c"%d\00"
@emptychar = global i8 0
declare i8* @malloc(i64)
declare noalias i8* @GC_malloc(i64)
declare void @GC_init()
declare i32 @strcmp(i8*, i8*)

declare i32 @fprintf(%__sFILE*, i8*, ...)
declare i32 @exit(i32) noreturn
@__stderrp = external global %__sFILE*
%__sFILE = type <{ i8*, i32, i32, i16, i16, i8, i8, i8, i8, %__sbuf, i32, i8, i8, i8, i8, i8*, i32 (i8*)*, i32 (i8*, i8*, i32)*, i64 (i8*, i64, i32)*, i32 (i8*, i8*, i32)*, %__sbuf, %__sFILEX*, i32, [3 x i8], [1 x i8], %__sbuf, i32, i8, i8, i8, i8, i64 }>
%__sFILEX = type opaque
%__sbuf = type <{ i8*, i32, i8, i8, i8, i8 }>
@"\01LC" = internal constant [3 x i8] c"%s\00"
@"\01LC1" = internal constant [5 x i8] c"null\00"
@"\01LC2" = internal constant [38 x i8] c"Error: input cannot exceed 1000 chars\00"
declare i32 @printf(i8*, ...)
declare i32 @getchar()




;#################################################################
;###                       basic types                         ###
;#################################################################


;;;;;; Any class ;;;;;
%class_Any = type {
  %class_Any*,                               ; null parent pointer
  i1 ( %obj_Any*, %obj_Any* )*               ; Booln equals(this,x)
}


%obj_Any = type {
  %class_Any*                                ; class ptr
}


@Any = global %class_Any {
  %class_Any* null,                          ; null superclass ptr
  i1 (%obj_Any*, %obj_Any*)* @Any_equals     ; equals
}




define i1 @Any_equals(%obj_Any* %this, %obj_Any* %x) {
  %any1 = alloca i1                  ; <i1*> 
  %this.addr = alloca %obj_Any*		; <%obj_Any**> 
  %x.addr = alloca %obj_Any*		    ; <%obj_Any**> 
  %ret = alloca i1, align 4		    ; <i1*> 
  store %obj_Any* %this, %obj_Any** %this.addr
  store %obj_Any* %x, %obj_Any** %x.addr
  %any2 = load %obj_Any** %this.addr	; <%obj_Any*> 
  %any3 = load %obj_Any** %x.addr		; <%obj_Any*> 
  %any4 = icmp eq %obj_Any* %any2, %any3		; <i1> 
  br i1 %any4, label %Label5, label %Label6

  ; <label>:5	        	                ; preds = %0
Label5:  
  store i1 1, i1* %ret
  br label %Label7

  ; <label>:6		                       ; preds = %0
Label6:  
  store i1 0, i1* %ret
  br label %Label7

  ; <label>:7		                        ; preds = %0
Label7:  
  store i1 0, i1* %ret
  %any8 = load i1* %ret		        ; <i1> 
  store i1 %any8, i1* %any1
  %any9 = load i1* %any1		        ; <i1> 
  ret i1 %any9
}
;;;;;; IO class ;;;;;
%class_IO = type {
  %class_Any*,                               ; parent pointer
  i1 ( %obj_IO*, %obj_Any* )*,               ; Booln equals(this,x)
  void ( %obj_IO*, i8* )*,     ; void abort(this, message)
  %obj_IO* ( %obj_IO*, i8* )*,    ; IO out(this, message)
  i1 ( %obj_IO*, %obj_Any* )*,          ; Boolean is_null(this, arg)
  i8* ( %obj_IO* )*              ; String in(this)
}


%obj_IO = type {
  %class_IO*                                ; class ptr
}


@IO = global %class_IO {
  %class_Any* @Any,                          ; superclass
  i1 ( %obj_IO*, %obj_Any* )* @IO_equals,    ; equals
  void ( %obj_IO*, i8* )* @IO_abort,    ; abort
  %obj_IO* ( %obj_IO*, i8* )* @IO_out,    ; out
  i1 ( %obj_IO*, %obj_Any* )* @IO_is_null,  ; is_null
  i8* ( %obj_IO* )* @IO_in              ; in
}


@IO_equals = alias i1 ( %obj_IO*, %obj_Any*)* bitcast (i1 (%obj_Any*, %obj_Any*)* @Any_equals to i1 ( %obj_IO*, %obj_Any*)*)


define void @IO_abort(%obj_IO* %this, i8* %message) {
  %this.addr = alloca %obj_IO*     ; <%obj_IO**> 
  %message.addr = alloca i8*      ; i8** 
  store %obj_IO* %this, %obj_IO** %this.addr
  store i8* %message, i8** %message.addr
  %1 = load %__sFILE** @__stderrp      ; <%__sFILE*> 
  %2 = load i8** %message.addr        ; i8* 
  %3 = call i32 (%__sFILE*, i8*, ...)* @fprintf(%__sFILE* %1, i8* getelementptr ([3 x i8]* @"\01LC", i32 0, i32 0), i8* %2)
  call i32 @exit(i32 1) noreturn
  unreachable ; No predecessors!
  ret void
}
define %obj_IO* @IO_out(%obj_IO* %this, i8* %message) nounwind {
     %1 = alloca %obj_IO*     ; <%obj_IO**> [#uses=2]
     %this.addr = alloca %obj_IO*     ; <%obj_IO**> [#uses=2]
     %message.addr = alloca i8*      ; i8** [#uses=2]
     store %obj_IO* %this, %obj_IO** %this.addr
     store i8* %message, i8** %message.addr
     %2 = load i8** %message.addr        ; i8*
     %3 = call i32 (i8*, ...)* @printf(i8* getelementptr ([3 x i8]* @"\01LC", i32 0, i32 0), i8* %2)     ; <i32>
     %4 = load %obj_IO** %this.addr       ; <%obj_IO*>
     store %obj_IO* %4, %obj_IO** %1
     %5 = load %obj_IO** %1       ; <%obj_IO*>
     ret %obj_IO* %5
 } 
 define i1 @IO_is_null(%obj_IO* %this, %obj_Any* %arg) nounwind {
     %1 = alloca i1     ; <i1*> 
     %this.addr = alloca %obj_IO*     ; <%obj_IO**> 
     %arg.addr = alloca %obj_Any*     ; <%obj_Any**> 
     %ret = alloca i1, align 4      ; <i1*> 
     store %obj_IO* %this, %obj_IO** %this.addr
     store %obj_Any* %arg, %obj_Any** %arg.addr
     %2 = load %obj_Any** %arg.addr       ; <%obj_Any*>
     %3 = icmp eq %obj_Any* %2, null      ; <i1> 
     br i1 %3, label %Label4, label %Label5
 
 ; <label>
Label4:     ; preds = %0
     store i1 1, i1* %ret
     br label %Label6
 
 ; <label>
Label5:     ; preds = %0
     store i1 0, i1* %ret
     br label %Label6
 
 ; <label>
Label6:     ; preds = %5, %4
     %4 = load i1* %ret     ; <i1> 
     store i1 %4, i1* %1
     %5 = load i1* %1       ; <i1> 
     ret i1 %5
 }
define i8* @IO_in(%obj_IO* %this) nounwind {
 	%1 = alloca i8*		; <i8**> 
 	%this.addr = alloca %obj_IO*		; <%obj_IO**> 
 	%numchar = alloca i32, align 4		; <i32*> 
 	%2 = alloca i8, align 1		; <i8*> 
 	%in = alloca i8*, align 8		; <i8**> 
 	%tmp = alloca [1000 x i8], align 1		; <[1000 x i8]*> 
 	store %obj_IO* %this, %obj_IO** %this.addr
 	store i32 0, i32* %numchar
 	br label %Label2
 
; <label>
Label2:		; preds = %21, %0
 	%3 = call i32 @getchar()		; <i32> 
 	%4 = trunc i32 %3 to i8		; <i8> 
 	store i8 %4, i8* %2
 	%5 = sext i8 %4 to i32		; <i32> 
 	%6 = icmp ne i32 %5, 10		; <i1> 
 	br i1 %6, label %Label7, label %Label22
 
; <label>
Label7:		; preds = %2
 	%7 = load i8* %2		; <i8> 
 	%8 = load i32* %numchar		; <i32> 
 	%9 = getelementptr [1000 x i8]* %tmp, i32 0, i32 0	 ; <i8*> 
 	%10 = sext i32 %8 to i64		; <i64> 
 	%11 = getelementptr i8* %9, i64 %10		; <i8*> 
 	store i8 %7, i8* %11
 	%12 = load i32* %numchar		; <i32> 
 	%13 = add i32 %12, 1		; <i32> 
 	store i32 %13, i32* %numchar
 	%14 = load i32* %numchar		; <i32> 
 	%15 = icmp sge i32 %14, 1000		; <i1> 
 	br i1 %15, label %Label17, label %Label21
 
; <label>
Label17:		; preds = %7
 	%16 = load %__sFILE** @__stderrp		; <%__sFILE*> 
 	%17 = call i32 (%__sFILE*, i8*, ...)* @fprintf(%__sFILE* %16, i8* getelementptr ([38 x i8]* @"\01LC2", i32 0, i32 0))		; <i32> 
    call i32 @exit(i32 1) noreturn
    unreachable ; No predecessors!
 	br label %Label21
 
; <label>
Label21:		; preds = %17, %7
 	br label %Label2
 
; <label>
Label22:		; preds = %2
 	%20 = load i32* %numchar		; <i32> 
 	%21 = getelementptr [1000 x i8]* %tmp, i32 0, i32 0	 ; <i8*> 
 	%22 = sext i32 %20 to i64		; <i64> 
 	%23 = getelementptr i8* %21, i64 %22		; <i8*> 
 	store i8 0, i8* %23
 	%24 = getelementptr [1000 x i8]* %tmp, i32 0, i32 0	 ; <i8*> 
 	store i8* %24, i8** %in
 	%25 = load i8** %in		; <i8*> 
 	store i8* %25, i8** %1
 	%26 = load i8** %1		; <i8*> 
 	ret i8* %26
}
;#################################################################
;###                     program types                         ###
;#################################################################


%class_A = type { %class_Any*, 
%obj_A* ( )*        ; _Constructor 
, i32 (%obj_A*, i32) *, i32 (%obj_A*) * }
%obj_A = type { %class_A* }
@A = global %class_A {%class_Any* @Any ,
%obj_A* ( )* @A_constructor 
, i32 (%obj_A*, i32) * @__method_A_incx, i32 (%obj_A*) * @__method_A_getval }

%class_B = type { %class_A*, 
%obj_B* ( )*        ; _Constructor 
, i32 (%obj_B*, i32) *, i32 (%obj_B*) * }
%obj_B = type { %class_B*, i32 }
@B = global %class_B {%class_A* @A ,
%obj_B* ( )* @B_constructor 
, i32 (%obj_B*, i32) * @__method_B_incy, i32 (%obj_B*) * @__method_B_getval }

%class_Main = type { %class_IO*, 
%obj_Main* ( )*        ; _Constructor 
 }
%obj_Main = type { %class_Main*, %obj_B*, i32 }
@Main = global %class_Main {%class_IO* @IO ,
%obj_Main* ( )* @Main_constructor 
 }



define i32 @__method_A_incx(%obj_A* %this, i32 %delta) {
	; Start ASSIGN
	%i0 = getelementptr %obj_A* %this, i32 0, i32 0
	; START Arithmetic operation (PLUSEXPR)
	; START ID load (x)
	%i1 = getelementptr %obj_A* %this, i32 0, i32 0
	%i2 = load i32 %i1
	; END ID load (x)
	; START ID load (delta)
	; START instantiating Int
	%i3 = alloca i32
	%i4 = getelementptr i32 null, i32 1
	%i5 = ptrtoint i32 %i4 to i64
	%i6 = call noalias i8* @GC_malloc(i64 %i5)
	%i7 = bitcast i8 * %i6 to i32
	store i32 %i7, i32* %i3
	%i8 = load i32* %i3
	; setting class pointer
	%i9 = getelementptr i32 %i8, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i9
	; Setting new Int to default (0)
	%i10 = getelementptr i32 %i8, i32 0, i32 1
	store i32 0, i32 * %i10
	; END instantiating Int
	%i11 = load i32* %i3
	; Getting first parameter to binop
	%i12 = getelementptr i32 %i2, i32 0, i32 1
	; Getting second parameter to binop
	%i13 = getelementptr Int delta, i32 0, i32 1
	%i14 = load i32 * %i12
	%i15 = load i32 * %i13
	%i16 = add i32  %i14, %i15
	%i17 = getelementptr i32 %i11, i32 0, i32 1
	store i32 %i16, i32 * %i17
	; END Arithmetic operation (PLUSEXPR)
	store i32* %i3, i32* %i0
	; End ASSIGN
	; START Int literal (0)
	; START instantiating Int
	%i18 = alloca i32
	%i19 = getelementptr i32 null, i32 1
	%i20 = ptrtoint i32 %i19 to i64
	%i21 = call noalias i8* @GC_malloc(i64 %i20)
	%i22 = bitcast i8 * %i21 to i32
	store i32 %i22, i32* %i18
	%i23 = load i32* %i18
	; setting class pointer
	%i24 = getelementptr i32 %i23, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i24
	; Setting new Int to default (0)
	%i25 = getelementptr i32 %i23, i32 0, i32 1
	store i32 0, i32 * %i25
	; END instantiating Int
	%i26 = load i32* %i18
	%i27 = getelementptr i32 %i26, i32 0, i32 1
	store i32 0, i32 * %i27
	; END Int literal (0)
	%i28 = bitcast i32* %i18 to i32
	ret i32 %i28
}

define i32 @__method_A_getval(%obj_A* %this) {
	; START ID load (x)
	%i29 = getelementptr %obj_A* %this, i32 0, i32 0
	%i30 = load i32 %i29
	; END ID load (x)
	ret i32 %i30
}

define i32 @__method_B_incy(%obj_B* %this, i32 %delta) {
	; Start ASSIGN
	%i31 = getelementptr %obj_B* %this, i32 0, i32 1
	; START Arithmetic operation (PLUSEXPR)
	; START ID load (y)
	%i32 = getelementptr %obj_B* %this, i32 0, i32 1
	%i33 = load i32 %i32
	; END ID load (y)
	; START ID load (delta)
	; START instantiating Int
	%i34 = alloca i32
	%i35 = getelementptr i32 null, i32 1
	%i36 = ptrtoint i32 %i35 to i64
	%i37 = call noalias i8* @GC_malloc(i64 %i36)
	%i38 = bitcast i8 * %i37 to i32
	store i32 %i38, i32* %i34
	%i39 = load i32* %i34
	; setting class pointer
	%i40 = getelementptr i32 %i39, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i40
	; Setting new Int to default (0)
	%i41 = getelementptr i32 %i39, i32 0, i32 1
	store i32 0, i32 * %i41
	; END instantiating Int
	%i42 = load i32* %i34
	; Getting first parameter to binop
	%i43 = getelementptr i32 %i33, i32 0, i32 1
	; Getting second parameter to binop
	%i44 = getelementptr Int delta, i32 0, i32 1
	%i45 = load i32 * %i43
	%i46 = load i32 * %i44
	%i47 = add i32  %i45, %i46
	%i48 = getelementptr i32 %i42, i32 0, i32 1
	store i32 %i47, i32 * %i48
	; END Arithmetic operation (PLUSEXPR)
	store i32* %i34, i32* %i31
	; End ASSIGN
	; START Int literal (0)
	; START instantiating Int
	%i49 = alloca i32
	%i50 = getelementptr i32 null, i32 1
	%i51 = ptrtoint i32 %i50 to i64
	%i52 = call noalias i8* @GC_malloc(i64 %i51)
	%i53 = bitcast i8 * %i52 to i32
	store i32 %i53, i32* %i49
	%i54 = load i32* %i49
	; setting class pointer
	%i55 = getelementptr i32 %i54, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i55
	; Setting new Int to default (0)
	%i56 = getelementptr i32 %i54, i32 0, i32 1
	store i32 0, i32 * %i56
	; END instantiating Int
	%i57 = load i32* %i49
	%i58 = getelementptr i32 %i57, i32 0, i32 1
	store i32 0, i32 * %i58
	; END Int literal (0)
	%i59 = bitcast i32* %i49 to i32
	ret i32 %i59
}

define i32 @__method_B_getval(%obj_B* %this) {
	; START Arithmetic operation (PLUSEXPR)
	; START ID load (x)
	%i60 = getelementptr %obj_B* %this, i32 0, i32 0
	%i61 = load i32 %i60
	; END ID load (x)
	; START ID load (y)
	%i62 = getelementptr %obj_B* %this, i32 0, i32 1
	%i63 = load i32 %i62
	; END ID load (y)
	; START instantiating Int
	%i64 = alloca i32
	%i65 = getelementptr i32 null, i32 1
	%i66 = ptrtoint i32 %i65 to i64
	%i67 = call noalias i8* @GC_malloc(i64 %i66)
	%i68 = bitcast i8 * %i67 to i32
	store i32 %i68, i32* %i64
	%i69 = load i32* %i64
	; setting class pointer
	%i70 = getelementptr i32 %i69, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i70
	; Setting new Int to default (0)
	%i71 = getelementptr i32 %i69, i32 0, i32 1
	store i32 0, i32 * %i71
	; END instantiating Int
	%i72 = load i32* %i64
	; Getting first parameter to binop
	%i73 = getelementptr i32 %i61, i32 0, i32 1
	; Getting second parameter to binop
	%i74 = getelementptr i32 %i63, i32 0, i32 1
	%i75 = load i32 * %i73
	%i76 = load i32 * %i74
	%i77 = add i32  %i75, %i76
	%i78 = getelementptr i32 %i72, i32 0, i32 1
	store i32 %i77, i32 * %i78
	; END Arithmetic operation (PLUSEXPR)
	%i79 = bitcast i32* %i64 to i32
	ret i32 %i79
}

define i32 @ll_main() {
	call void @GC_init()

	; START instantiating Main
	%i80 = alloca %obj_Main*
	%i81 = getelementptr %obj_Main* null, i32 1
	%i82 = ptrtoint %obj_Main* %i81 to i64
	%i83 = call noalias i8* @GC_malloc(i64 %i82)
	%i84 = bitcast i8 * %i83 to %obj_Main*
	store %obj_Main* %i84, %obj_Main** %i80
	%i85 = load %obj_Main** %i80
	; setting class pointer
	%i86 = getelementptr %obj_Main* %i85, i32 0, i32 0
	store %class_Main* @Main, %class_Main** %i86
	; START attribute b:B of Main
	%i87 = getelementptr %obj_Main* %i85, i32 0, i32 1
	store %obj_B* null, %obj_B** %i87
	; END attribute b:B of Main
	; START attribute i:Int of Main
	%i88 = getelementptr %obj_Main* %i85, i32 0, i32 2
	; START instantiating Int
	%i89 = alloca i32
	%i90 = getelementptr i32 null, i32 1
	%i91 = ptrtoint i32 %i90 to i64
	%i92 = call noalias i8* @GC_malloc(i64 %i91)
	%i93 = bitcast i8 * %i92 to i32
	store i32 %i93, i32* %i89
	%i94 = load i32* %i89
	; setting class pointer
	%i95 = getelementptr i32 %i94, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i95
	; Setting new Int to default (0)
	%i96 = getelementptr i32 %i94, i32 0, i32 1
	store i32 0, i32 * %i96
	; END instantiating Int
	%i97 = load i32* %i89
	store i32 %i97, i32* %i88
	; END attribute i:Int of Main
	; Initialize b:B to introduced value
	%i98 = getelementptr %obj_Main* %i85, i32 0, i32 1
	%i99 = load %obj_Main** %i80
	; START instantiating B
	%i100 = alloca %obj_B*
	%i101 = getelementptr %obj_B* null, i32 1
	%i102 = ptrtoint %obj_B* %i101 to i64
	%i103 = call noalias i8* @GC_malloc(i64 %i102)
	%i104 = bitcast i8 * %i103 to %obj_B*
	store %obj_B* %i104, %obj_B** %i100
	%i105 = load %obj_B** %i100
	; setting class pointer
	%i106 = getelementptr %obj_B* %i105, i32 0, i32 0
	store %class_B* @B, %class_B** %i106
	; START attribute y:Int of B
	%i107 = getelementptr %obj_B* %i105, i32 0, i32 1
	; START instantiating Int
	%i108 = alloca i32
	%i109 = getelementptr i32 null, i32 1
	%i110 = ptrtoint i32 %i109 to i64
	%i111 = call noalias i8* @GC_malloc(i64 %i110)
	%i112 = bitcast i8 * %i111 to i32
	store i32 %i112, i32* %i108
	%i113 = load i32* %i108
	; setting class pointer
	%i114 = getelementptr i32 %i113, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i114
	; Setting new Int to default (0)
	%i115 = getelementptr i32 %i113, i32 0, i32 1
	store i32 0, i32 * %i115
	; END instantiating Int
	%i116 = load i32* %i108
	store i32 %i116, i32* %i107
	; END attribute y:Int of B
	; Initialize y:Int to introduced value
	%i117 = getelementptr %obj_B* %i105, i32 0, i32 1
	%i118 = load %obj_B** %i100
	; START Int literal (7)
	; START instantiating Int
	%i119 = alloca i32
	%i120 = getelementptr i32 null, i32 1
	%i121 = ptrtoint i32 %i120 to i64
	%i122 = call noalias i8* @GC_malloc(i64 %i121)
	%i123 = bitcast i8 * %i122 to i32
	store i32 %i123, i32* %i119
	%i124 = load i32* %i119
	; setting class pointer
	%i125 = getelementptr i32 %i124, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i125
	; Setting new Int to default (0)
	%i126 = getelementptr i32 %i124, i32 0, i32 1
	store i32 0, i32 * %i126
	; END instantiating Int
	%i127 = load i32* %i119
	%i128 = getelementptr i32 %i127, i32 0, i32 1
	store i32 7, i32 * %i128
	; END Int literal (7)
	%i129 = bitcast i32* %i119 to i32
	store i32 %i129, i32* %i117
	; END instantiating B
	%i130 = load %obj_B** %i100
	store %obj_B* %i130, %obj_B** %i98
	; Initialize i:Int to introduced value
	%i131 = getelementptr %obj_Main* %i85, i32 0, i32 2
	%i132 = load %obj_Main** %i80
	; START Method call (getval)
	; START ID load (b)
	%i133 = getelementptr %obj_Main* %i132, i32 0, i32 1
	%i134 = load %obj_B* %i133
	; END ID load (b)
	; Get pointer to class of object
	%i135 = bitcast %obj_B %i134 to %obj_B*
	%i136 = getelementptr %obj_B* %i135, i32 0, i32 0
	%i137 = load %class_B** %i136
	; getting method getval():Int of B
	%i138 = getelementptr %class_B* %i137, i32 0, i32 2
	%i139 = load i32 (%obj_B*) ** %i138
	%i140 = bitcast %obj_B %i134 to %obj_B*
	; calling method getval():Int
	%i141 = call i32 %i139(%obj_B* %i140)
	; END Method call (getval)
	store i32 %i141, i32* %i131
	; END instantiating Main
	%i142 = load %obj_Main** %i80
	; START Method call (incx)
	; START ID load (b)
	%i143 = getelementptr obj_Main* main0, i32 0, i32 1
	%i144 = load %obj_B* %i143
	; END ID load (b)
	; START Int literal (1)
	; START instantiating Int
	%i145 = alloca i32
	%i146 = getelementptr i32 null, i32 1
	%i147 = ptrtoint i32 %i146 to i64
	%i148 = call noalias i8* @GC_malloc(i64 %i147)
	%i149 = bitcast i8 * %i148 to i32
	store i32 %i149, i32* %i145
	%i150 = load i32* %i145
	; setting class pointer
	%i151 = getelementptr i32 %i150, i32 0, i32 0
	store %class_Int* @Int, %class_Int** %i151
	; Setting new Int to default (0)
	%i152 = getelementptr i32 %i150, i32 0, i32 1
	store i32 0, i32 * %i152
	; END instantiating Int
	%i153 = load i32* %i145
	%i154 = getelementptr i32 %i153, i32 0, i32 1
	store i32 1, i32 * %i154
	; END Int literal (1)
	%i155 = load i32* %i145
	; Get pointer to class of object
	%i156 = bitcast %obj_B %i144 to %obj_B*
	%i157 = getelementptr %obj_B* %i156, i32 0, i32 0
	%i158 = load %class_B** %i157
	; getting method incx(delta:Int):Int of A
	%i159 = getelementptr %class_B* %i158, i32 0, i32 1
	%i160 = load i32 (%obj_A*, i32) ** %i159
	%i161 = bitcast %obj_B %i144 to %obj_A*
	; calling method incx(delta:Int):Int
	%i162 = call i32 %i160(%obj_A* %i161, i32 %i155)
	; END Method call (incx)
	; START Method call (out)
	; START String literal (hello)
	; START instantiating String
	%i163 = alloca %obj_String*
	%i164 = getelementptr %obj_String* null, i32 1
	%i165 = ptrtoint %obj_String* %i164 to i64
	%i166 = call noalias i8* @GC_malloc(i64 %i165)
	%i167 = bitcast i8 * %i166 to %obj_String*
	store %obj_String* %i167, %obj_String** %i163
	%i168 = load %obj_String** %i163
	; setting class pointer
	%i169 = getelementptr %obj_String* %i168, i32 0, i32 0
	store %class_String* @String, %class_String** %i169
	; Setting new String to default (empty)
	%i170 = getelementptr %obj_String* %i168, i32 0, i32 1
	store i32 1, i32 * %i170
	%i171 = getelementptr %obj_String* %i168, i32 0, i32 2
	%i173 = call noalias i8* @GC_malloc(i64 1)
	%i172 = bitcast i8 * %i173 to [1 x i8]*
	store [1 x i8] c"\00", [1 x i8]* %i172
	%i174 = bitcast [1 x i8]* %i172 to i8 *
	store i8 * %i174, i8 ** %i171
	; END instantiating String
	%i175 = load %obj_String** %i163
	%i176 = getelementptr %obj_String* %i175, i32 0, i32 1
	store i32 6, i32 * %i176
	%i177 = getelementptr %obj_String* %i175, i32 0, i32 2
	%i179 = call noalias i8* @GC_malloc(i64 6)
	%i178 = bitcast i8 * %i179 to [6 x i8]*
	store [6 x i8] c"hello\00", [6 x i8]* %i178
	%i180 = bitcast [6 x i8]* %i178 to i8 *
	store i8 * %i180, i8 ** %i177
	; END String literal (hello)
	%i181 = load %obj_String** %i163
	; Get pointer to class of object
	%i182 = bitcast obj_Main* main1 to %obj_Main*
	%i183 = getelementptr %obj_Main* %i182, i32 0, i32 0
	; getting method out(arg:String):IO of IO
	%i184 = getelementptr %class_Main* %i183, i32 0, i32 -1
	%i185 = load %obj_IO* (%obj_IO*, %obj_String*) ** %i184
	%i186 = bitcast obj_Main* main1 to %obj_IO*
	; calling method out(arg:String):IO
	%i187 = call %obj_IO* %i185(%obj_IO* %i186, %obj_String* %i181)
	; END Method call (out)
	ret i32 0
}


