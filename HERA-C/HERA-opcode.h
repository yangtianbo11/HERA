static void manually_execute_undefined_HERA_op(int op);

#if ! defined OPCODE
#define OPCODE(N)	manually_execute_undefined_HERA_op(N);
#endif

/*
 * The following makes it possible to simulate an instruction
 *  for an extended HERA processor, e.g. one with an INPUT
 *  or OUTPUT instruction.
 *
 * For now, this can only be done in the debugger,
 *  by setting a breakpoint in this function
 *  (i.e., by double-clicking on the left margin in Eclipse,
 *         or using the text GDB command
 *	     break manually_execute_undefined_HERA_op
 *  ).
 *
 * Default behaviour for executions not intercepted by the
 *  debugger can be controlled by the preprocessor variable
 *    EXECUTE_UNDEFINED_HERA_OPS_AS_NO_OPS_SILENTLY
 *  or (in the debugger, if the above is not #defined), the variable
 *    execute_undefined_hera_ops_as_no_ops_silently 
 */

#if ! defined EXECUTE_UNDEFINED_HERA_OPS_AS_NO_OPS_SILENTLY
#define EXECUTE_UNDEFINED_HERA_OPS_AS_NO_OPS_SILENTLY  execute_undefined_hera_ops_as_no_ops_silently 
#endif

static void manually_execute_undefined_HERA_op(int op)
{
	bool set_this_to_true_in_the_debugger = false;  // set a breakpoint here
	static bool execute_undefined_hera_ops_as_no_ops_silently = false;
	static int  useless_count_to_shut_the_compiler_warnings   = 0;

	if (set_this_to_true_in_the_debugger) {
		;  // use the debugger to change registers, flags, memory, etc...
	} else {
		if (execute_undefined_hera_ops_as_no_ops_silently)
			useless_count_to_shut_the_compiler_warnings++;
#if !EXECUTE_UNDEFINED_HERA_OPS_AS_NO_OPS_SILENTLY
		Herror("WARNING: executing undefined HERA opcode", false);
#endif
	}
}

