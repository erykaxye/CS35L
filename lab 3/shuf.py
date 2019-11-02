#!/usr/bin/python 

'''
Name: Erica Xie
ID: 404920875
Lab 7 
'''

import sys
import random
import string 
from optparse import OptionParser

class shuf: 
    def __init__(self, input_list, count, repeat): 
        self.input_list = input_list 
        self.repeat = repeat 
        self.count = count

        #shuffles input list 
        random.shuffle(self.input_list) 
       
    def shuffle_lines(self): 
        #checks if input list is empty 
        if len(self.input_list) == 0: 
            return 
        #checks if repeat is true, if so can repeat outputs 
        if self.repeat: 
            while self.count > 0: 
                sys.stdout.write(random.choice(self.input_list))
                self.count -= 1
        #else output COUNT lines of shuffled input list
        else: 
            for i in range(self.count): 
                sys.stdout.write(self.input_list[i])
               
def main():
    usage_msg = '''%prog [OPTION] [OPTION] [OPTION]... FILE

output lines in a random order from FILE or from range of numbers.'''

    parser = OptionParser(usage=usage_msg)
    parser.add_option('-i', '--input-range', 
                      metavar='LO-HI', 
                      action='store', 
                      dest='input_range', 
                      default='', 
                      help='output shuffled LO-HI numbers')
    parser.add_option('-r', '--repeat', 
                      metavar='REPEAT',
                      action='store_true', 
                      dest='repeat', 
                      default=False, 
                      help='output lines can be repeated')
    parser.add_option('-n', '--head-count', 
                      metavar='COUNT', 
                      action='store',
                      dest='count',
                      default=sys.maxsize, #imitates infinity 
                      help='output at most COUNT lines')

    options, args = parser.parse_args(sys.argv[1:]) 

    #checks -n, --head-count option
    try:
        count = int(options.count)
    except:
        parser.error("invalid COUNT: {0}".
                     format(options.count))

    if count < 0:
        parser.error("negative count: {0}".
                     format(count))

    #checks -r, --repeat option 
    repeat = options.repeat
    
    #checks -i, --input-range option
    if options.input_range != '': 
        if len(args) != 0: 
             parser.error("extra operand '{0}'".
                          format(args[0]))
        try: 
            #splits HI-LO into two numbers 
            temp_range = options.input_range.split('-')
            input_range = ['1', '2']
            for i in range(0,2):
                input_range[i] = int(temp_range[i])
        except: 
            parser.error("invalid LO-HI: {0}".
                         format(options.input_range))

        start = input_range[0]
        end = input_range[1] 
        
        #checks if range is valid
        if start > end:
             parser.error("invalid LO-HI1: {0}".
                         format(options.input_range))
        if start < 0 or end < 0:
            parser.error("invalid LO-HI2: {0}".
                         format(options.input_range))
        
        #converts input range into a list of ints 
        input_list = list(range(start, end+1))
        for i in range(len(input_list)):
            input_list[i] = str(input_list[i]) + '\n'
 
    elif len(args) == 1: 
        #puts lines of files into input list
        try: 
            f = open(args[0], 'r')
            input_list = f.readlines()
            f.close()
        except IOError as err:
            parser.error("I/O error({0}): {1}".
                         format(errno, strerror))

    elif len(args) == 0 or (len(args) == 1 and args[0] == '-'): 
        #read lines from stdin into input list
        try: 
            input_list = sys.stdin.readlines()
        except: 
            parser.error("no standard input")

    else: 
         parser.error("wrong number of operands")

    #checks if count is valid and not repeating
    if count > len(input_list) and not repeat: 
        count = len(input_list)
    #if it does repeat, then count is infinite?
    #but at the same time if theres a count then it should print COUNT lines

    try:
        generator = shuf(input_list, count, repeat)
        generator.shuffle_lines()    
    except:
        parser.error("cannot shuffle lines")

if __name__ == '__main__':
    main()
