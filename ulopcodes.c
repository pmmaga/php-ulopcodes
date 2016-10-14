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
#include "ext/standard/info.h"
#include "Zend/zend.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm_opcodes.h"
#include "php_ulopcodes.h"

/* If you declare any globals in php_ulopcodes.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ulopcodes)
*/

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ulopcodes.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ulopcodes_globals, ulopcodes_globals)
    STD_PHP_INI_ENTRY("ulopcodes.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ulopcodes_globals, ulopcodes_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ proto string confirm_ulopcodes_compiled(string arg)
   Dummy function to be found in the oparray */
PHP_FUNCTION(ulopcodes_emit)
{

}
/* }}} */

/* {{{ php_ulopcodes_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ulopcodes_init_globals(zend_ulopcodes_globals *ulopcodes_globals)
{
	ulopcodes_globals->global_value = 0;
	ulopcodes_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ulopcodes)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ulopcodes support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
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
	NULL,
	NULL,
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
	unsigned int i;

	if(op_array->function_name) {
		php_printf("Function: %s\n", op_array->function_name->val);
	} else {
		php_printf("Function: (no name)\n");
	}

	for (i = 0; i < op_array->last; i++) {
		if(op_array->opcodes[i].opcode == ZEND_INIT_FCALL) {
			php_printf("Opcode: %s %d\n", zend_get_opcode_name(op_array->opcodes[i].opcode), op_array->opcodes[i].op2.num);
		} else if (op_array->opcodes[i].opcode < ZEND_VM_LAST_OPCODE) {
			php_printf("Opcode: %s\n", zend_get_opcode_name(op_array->opcodes[i].opcode));
		} else {
			php_printf("Opcode: UNKNOWN\n");
		}
	}
}

ZEND_DLEXPORT void ulop_fcall_begin(zend_execute_data *frame)
{
	php_printf("fcall");
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
	ulop_fcall_begin,	/* fcall_begin_handler_func_t */
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
