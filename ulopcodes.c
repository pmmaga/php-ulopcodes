/*
	Copyright (c) 2016 Pedro Magalhães

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend.h"
#include "Zend/zend_types.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm_opcodes.h"
#include "php_ulopcodes.h"

ZEND_DECLARE_MODULE_GLOBALS(ulopcodes)

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ulopcodes.enabled", 			"0", PHP_INI_ALL, OnUpdateBool, enabled, zend_ulopcodes_globals, ulopcodes_globals)
	STD_PHP_INI_ENTRY("ulopcodes.dump_oparray", 	"0", PHP_INI_ALL, OnUpdateBool, dump_oparray, zend_ulopcodes_globals, ulopcodes_globals)
PHP_INI_END()
/* }}} */

/* {{{ ulopcodes_init_globals
 */
static void ulopcodes_init_globals(zend_ulopcodes_globals *ulopcodes_globals)
{
	ulopcodes_globals->enabled = 0;
	ulopcodes_globals->dump_oparray = 0;
}
/* }}} */

/* {{{ proto string confirm_ulopcodes_compiled(string arg)
   Dummy function to be found in the oparray */
PHP_FUNCTION(ulopcodes_emit)
{

}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ulopcodes)
{
	unsigned int i;
	/*
		Register the opcodes as constants for the user
	*/
	for (i = 0; i <= ZEND_VM_LAST_OPCODE; i++) { 
		const char *name = zend_get_opcode_name(i); 
		if (name) {
			zend_register_long_constant(name, strlen(name), i, CONST_CS|CONST_PERSISTENT, module_number);
		} 
	}

	ZEND_INIT_MODULE_GLOBALS(ulopcodes, ulopcodes_init_globals, NULL);
	REGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ulopcodes)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ulopcodes)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ulopcodes support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ ulopcodes_functions[]
 *
 * Every user visible function must have an entry in ulopcodes_functions[].
 */
const zend_function_entry ulopcodes_functions[] = {
	PHP_FE(ulopcodes_emit,	NULL)
	PHP_FE_END	/* Must be the last line in ulopcodes_functions[] */
};
/* }}} */

/* {{{ ulopcodes_module_entry
 */
zend_module_entry ulopcodes_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_ULOPCODES_NAME,
	ulopcodes_functions,
	PHP_MINIT(ulopcodes),
	PHP_MSHUTDOWN(ulopcodes),
	NULL,
	NULL,
	PHP_MINFO(ulopcodes),
	PHP_ULOPCODES_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

ZEND_DLEXPORT int ulop_startup(zend_extension *extension)
{
	return zend_startup_module(&ulopcodes_module_entry);
}

ZEND_DLEXPORT void ulop_oparray_h(zend_op_array *op_array)
{
	ZEND_TSRMLS_CACHE_UPDATE();

	if (ULOP_G(enabled)) {
		unsigned int i;

		if (ULOP_G(dump_oparray)) {
			if(op_array->function_name) {
				php_printf("Function: %s\n", op_array->function_name->val);
			} else {
				php_printf("Function: (no name)\n");
			}
		}

		for (i = 0; i < op_array->last; i++) {
			/*
				If we find a call to the dummy function on the opcodes
			*/
			if (op_array->opcodes[i].opcode == ZEND_INIT_FCALL &&
				op_array->opcodes[i].op2_type == IS_CONST &&
				Z_TYPE(ULOP_OP2_CONSTANT(op_array, i)) == IS_STRING &&
				Z_STRLEN(ULOP_OP2_CONSTANT(op_array, i)) == strlen("ulopcodes_emit") &&
				strcmp(Z_STRVAL(ULOP_OP2_CONSTANT(op_array, i)), "ulopcodes_emit") == 0 //&& false
			) {
				zend_op new_op;
				MAKE_NOP(&new_op);

				unsigned int j = i + 1;
				unsigned int found = 0;

				MAKE_NOP(&op_array->opcodes[i]);

				/*
					Get the operands from the SEND_VAL calls that follow
				*/
				while ((op_array->opcodes[j].opcode != ZEND_DO_ICALL) && (j < op_array->last - 1)) {
					if (op_array->opcodes[j].opcode == ZEND_SEND_VAL || op_array->opcodes[j].opcode == ZEND_SEND_VAR) {
						if (found == 0) {
							/*
								Get the user's opcode
							*/
							if (op_array->opcodes[j].opcode == ZEND_SEND_VAL &&
								Z_TYPE(ULOP_OP1_CONSTANT(op_array, j)) == IS_LONG &&
								Z_LVAL(ULOP_OP1_CONSTANT(op_array, j)) < ZEND_VM_LAST_OPCODE
							) {
								new_op.opcode = Z_LVAL(ULOP_OP1_CONSTANT(op_array, j));
							} else if (op_array->opcodes[j].opcode == ZEND_SEND_VAR) {
								php_error(E_ERROR, "Please use constants for the opcode passed to ulopcodes_emit.");
							} else {
								php_error(E_ERROR, "Unknown opcode passed to ulopcodes_emit.");
							}
							MAKE_NOP(&op_array->opcodes[j]);
						} else if (found == 1) {
							/*
								Get the first op
							*/
							if (op_array->opcodes[j].op1_type != IS_UNUSED) {
								if (op_array->opcodes[j].op1_type == IS_CONST &&
									Z_TYPE(ULOP_OP1_CONSTANT(op_array, j)) == IS_NULL
								) {
									new_op.op1_type = IS_UNUSED;
								} else {
									new_op.op1_type = op_array->opcodes[j].op1_type;
								}
								new_op.op1 = op_array->opcodes[j].op1;
							}
							MAKE_NOP(&op_array->opcodes[j]);
						} else if (found == 2) {
							/*
								Get the second op
							*/
							if (op_array->opcodes[j].op1_type != IS_UNUSED) {
								if (op_array->opcodes[j].op1_type == IS_CONST &&
									Z_TYPE(ULOP_OP1_CONSTANT(op_array, j)) == IS_NULL
								) {
									new_op.op2_type = IS_UNUSED;
								} else {
									new_op.op2_type = op_array->opcodes[j].op1_type;
								}
								new_op.op2 = op_array->opcodes[j].op1;
							}
							MAKE_NOP(&op_array->opcodes[j]);
						} else if (found == 3) {
							/*
								Get the extended value
							*/
							if (op_array->opcodes[j].op1_type == IS_CONST &&
								Z_TYPE(ULOP_OP1_CONSTANT(op_array, j)) == IS_LONG
							) {
								new_op.extended_value = Z_LVAL(ULOP_OP1_CONSTANT(op_array, j));
							} else {
								php_error(E_ERROR, "Please use a number for the extended_value passed to ulopcodes_emit.");
							}
							MAKE_NOP(&op_array->opcodes[j]);
						}
						found++;
					}
					j++;
				}
				if (found > 0) {
					op_array->opcodes[j].opcode = new_op.opcode;
					op_array->opcodes[j].op1_type = new_op.op1_type;
					op_array->opcodes[j].op1 = new_op.op1;
					op_array->opcodes[j].op2_type = new_op.op2_type;
					op_array->opcodes[j].op2 = new_op.op2;
					op_array->opcodes[j].extended_value = new_op.extended_value;
				}
			}

			if (ULOP_G(dump_oparray)) {
				if (op_array->opcodes[i].opcode < ZEND_VM_LAST_OPCODE) {
					if (op_array->opcodes[i].opcode == ZEND_INIT_FCALL && op_array->opcodes[i].op2_type == IS_CONST) {
						php_printf("Fcall Target: %s\n", op_array->literals[op_array->opcodes[i].op2.constant].value.str->val);
					}
					php_printf("Opcode: %s (%d - %d) (%d - %d) %d\n",
						zend_get_opcode_name(op_array->opcodes[i].opcode),
						op_array->opcodes[i].op1_type,
						op_array->opcodes[i].op1.num,
						op_array->opcodes[i].op2_type,
						op_array->opcodes[i].op2.num,
						op_array->opcodes[i].extended_value
					);
				} else {
					php_printf("Opcode: UNKNOWN\n");
				}
			}
		}

		if (ULOP_G(dump_oparray)) {
			php_printf("End function\n");
		}
	}
}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API    ZEND_DLEXPORT
#endif
ZEND_EXTENSION();

ZEND_DLEXPORT zend_extension zend_extension_entry = {
	PHP_ULOPCODES_NAME,
	PHP_ULOPCODES_VERSION,
	PHP_ULOPCODES_AUTHOR,
	PHP_ULOPCODES_URL,
	PHP_ULOPCODES_COPYRIGHT,
	ulop_startup,		/* startup_func_t */
	NULL,				/* shutdown_func_t */
	NULL,           	/* activate_func_t */
	NULL,           	/* deactivate_func_t */
	NULL,           	/* message_handler_func_t */
	ulop_oparray_h,     /* op_array_handler_func_t */
	NULL, 				/* statement_handler_func_t */
	NULL,				/* fcall_begin_handler_func_t */
	NULL,				/* fcall_end_handler_func_t */
	NULL,   			/* op_array_ctor_func_t */
	NULL,           	/* op_array_dtor_func_t */
	STANDARD_ZEND_EXTENSION_PROPERTIES
};

#ifdef COMPILE_DL_ULOPCODES
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ulopcodes)
#endif
