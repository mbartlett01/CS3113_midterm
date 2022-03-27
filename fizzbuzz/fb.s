.section .data	
	#Create the Strings that will be printed
	fizz: 
		.ascii "Fizz"
	buzz:
		.ascii "Buzz"
	newline:
		.ascii "\n"
.section .bss
	#Buffer used for printing values
	.comm buff, 128

.section .text
.globl _start
_start:
	#Initialize loop variable and divisible flag
	movq $1, %rdi
	movq $0, %rbx

	#Create the loop 
	_start_loop:
		#Calculate rdi mod 3 (Result stored in %rdx)
		movq $0, %rdx
		movq %rdi, %rax
		movq $3, %rcx
		divq %rcx

		#Print Fizz if the result of the mod is 0
		cmpq $0, %rdx
		jne no_fizz
		movq $fizz, %rsi
		movq $4, %rdx
		call _print
		#Set %rbx to 1 to use as a flag
		movq $1, %rbx
		no_fizz:

		#Calculate rdi mod 5 (Result stored in %rdx)
		movq $0, %rdx
		movq %rdi, %rax
		movq $5, %rcx
		divq %rcx

		#Print Buzz if the result of the mod is 0
		cmpq $0, %rdx
		jne no_buzz
		movq $buzz, %rsi
		movq $4, %rdx
		call _print
		#Set %rbx to 1 to use as a flag
		movq $1, %rbx
		no_buzz:
		
		#Print the number if it is not divisible by 3 or 5 (tracked by %rbx register)
		cmpq $0, %rbx
		jne no_int
		call _print_int
		no_int:

		#Print the newline character seperately to avoid having to do concatenation of Strings
		movq $newline, %rsi
		movq $1, %rdx
		call _print

		#Reset %rbx
		movq $0, %rbx

		#Increment then loop if needed
		incq %rdi
		cmpq $100, %rdi
		jg _end_loop
		jmp _start_loop

	_end_loop:
	
	#Exit the program
	jmp _exit_x86_64bit

#Exit code taken from ex_control_flow_x86_64bit by Richard Veras (Found on Canvas)
#This code is not modified since it is pretty standard and is only 3 lines
_exit_x86_64bit:
	# This is slightly different because in x86_64 (64bit) we get the
	# syscall instruction, which provides more flexibility than int (interrupt)
	# so it is the defacto way for making system calls in 64bit binaries, in linux,
	# on systems with the x86_64 instruction set architecture.
	movq $60, %rax  # call number 60 -> exit
	movq $0x0, %rdi # return 0
	syscall #run kernel

#Printing Code using system calls comes from https://stackoverflow.com/questions/45835456/printing-an-integer-as-a-string-with-att-syntax-with-linux-system-calls-instea
#I chose to use system calls over the print function since I already modiefied and implemented this in project 1, so I am familiar with this
#This code has been modified to use a buffer rather than the stack (in order to allow me to easily save the ergisters on the stack instead)
#and it has had the registers used changed as well as a few changes in the order of instructions and an added shifting of the string (Since the 
#buffer does not automatically invert the string like the stack does)
#Pass the integer value to print in %rdi
.p2align 4	#Change the allignment so that the buffer works
.type _print_int, @function
_print_int:
	#Standard function initialization
	pushq	%rbp
	movq	%rsp, %rbp

	#Save the value of the registers we use
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi

	movq $0, %rsi  #Set %rsi to zero

    mov    $10, %ecx            # move 10 in rcx to use in division (ecx becuase division is weird)

    mov    %rdi, %rax           # function arg arrives in RDI; we need it in RAX for div
	.Ltoascii_digit:                # do{
		xor    %edx, %edx			#Clear edx
		div    %rcx                  #  rax = rdx:rax / 10.  rdx = remainder

		add    $'0', %rdx           # integer to ASCII.
		incq   %rsi					#Add 1 to the number of digits written

		#Shift the previous values in the buffer down by 1
		mov %rsi, %rdi
		_shift_loop:
			decq %rdi
			movq buff(,%rdi, 8), %rbx
			incq %rdi
			movq %rbx, buff(,%rdi, 8)
			decq %rdi
			#Check to see if we need to break out of the loop
			cmp $0, %rdi
			je _end_shift_loop
			jmp _shift_loop
		_end_shift_loop:

		#Put the new value in the buffer
		movq %rdx, buff

		test   %rax, %rax
    jnz  .Ltoascii_digit        # } while(value != 0)

    # Then print the whole string with one system call
    mov $1, %rax			# write system call
    mov $1, %rdi			# use standard output
    imul $8, %rsi, %rdx		# length is stored in %rdx (lenght is 8 bits times the number of characters)
	mov $buff, %rsi			# Message is located at buff
    syscall					# Output the number using a system call
    
	#Restore the value of all of the registers we use
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax

	#Restore stack and return
	movq %rbp, %rsp
	popq %rbp
    ret

#Function prints a string at a given memory location of a fixed length
#Pass the message in %rsi
#Pass the length of the message in %rdx
#Again, I am using system calls since I am more familiar with them due to testing them extensively in project 1
.type _print, @function
_print:
	#Standard function initialization
	pushq	%rbp
	movq	%rsp, %rbp

	#Save the registers we use
	pushq %rax
	pushq %rdi

	#Fill in the arguments to the system call (string position and length should alread be in correct registers)
    mov $1, %rax	# Write system call
    mov $1, %rdi	# Use standard output
    syscall			# Output the string using a system call

	#Restore the registers we use
    popq %rdi
	popq %rax

	#Restore stack and return
	movq %rbp, %rsp
	popq %rbp
    ret
