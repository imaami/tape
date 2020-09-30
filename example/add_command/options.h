#ifndef TAPE__OPTIONS_H__INCLUDED_
#define TAPE__OPTIONS_H__INCLUDED_

/*     Tag      -x      --xxx           Arg.    Help text           */
OPTION(Fifo,    'f',    "fifo",         1,      "Target dispatcher's named pipe.")
OPTION(Handler, 'H',    "handler",      1,      "Name of the handler that will receive the command.")
OPTION(Name,    'n',    "name",         1,      "Name that will be used to trigger the command.")

#endif /* TAPE__OPTIONS_H__INCLUDED_ */
