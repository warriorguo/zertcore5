import hashlib
import os
import pickle
import functools
import glob

def all_cpp(path, avoid):
	src_files = []
	
	for root, dirs, files in os.walk(path):
		is_avoid = False
		for i in avoid:
			if (root.find(i) != -1):
				is_avoid = True
				break
				
		if is_avoid : continue
		
		src_files += glob.glob(root + '/*.cpp')
	
	return src_files
		

def save_file(filename, content, hash):
	fp = open(filename, 'w')
	fp.write(content)
	fp.close()
	
	hash = hashlib.md5(content + hash).hexdigest().upper()
	
	fp = open(filename + ".key", 'w')
	fp.write(hash)
	fp.close()

def check_file(filename, hash):
	content = ''
	
	try:
		fp = open(filename, 'r')
		content = fp.read()
	except IOError:
		return False
	except:
		fp.close()
		return False
		
	fp.close()
	key = ''
	
	try:
		fp = open(filename + ".key", 'r')
		key = fp.read()
	except IOError:
		return False
	except:
		fp.close()
		return False
	fp.close()
		
	hash = hashlib.md5(content + hash).hexdigest().upper()
	if key == hash:
		return True
	
	return False
	

def get_hash(*pn):
	content = ''
	for p in pn:
		content += str(p)
	
	return hashlib.md5(content).hexdigest().upper()

def generate_callwrapper_ipp(filename, tpl, params_amount, fix = False, assign = False):
    hash = get_hash(tpl, params_amount)
    
    if check_file(filename, hash):
    	return
    
    def handle_content(file, count, params_str, const_flags):
        template_str = ""
        call_str = ""
        bind_params_str = ""
        
        for i in range(0, count):
            template_str += "typename T%d," % i
            bind_params_str += "_%d," % (i + 1)
            
            if not assign:
                call_str += "__p%d," % i
#           elif const_flags[i]:
#               call_str += "__p%d," % i
            else:
                call_str += "assign(__p%d, AssignTraits<T%d>())," % (i, i)

        template_str = template_str[0:-1] + ''
        call_str = call_str[0:-1]
        bind_params_str = bind_params_str[0:-1] + ''
        
        content = tpl;
        content = content.replace("{template}", template_str)
        content = content.replace("{call}", call_str)
        content = content.replace("{params}", params_str)
        content = content.replace("{bind_params}", bind_params_str)
        
        file['content'] += content + "\n"
    
    def statue_gen(file, max, count, str, const_flags):
        prefix = ['', 'const', 'volatile', 'const volatile']
        
        if max <= count:
            handle_content(file, count, str, const_flags)
            return
        
        for p in prefix:
            flags = const_flags[:]

            if p.find('const') == -1:
                flags.append(0)
            else:
                flags.append(1)
        	
            if len(str) > 0:
                statue_gen(file, max, count + 1, "%s,%s T%d & __p%d"%(str,p,count, count), flags)
            else:
                statue_gen(file, max, count + 1, "%s T%d & __p%d"%(p,count, count), flags)
            
    file = {'content':''}
    
    if fix == False:
        for i in range(1, params_amount + 1):
            statue_gen(file, i, 0, '', [])
    else:
        statue_gen(file, params_amount, 0, '', [])
    
    save_file(filename, file['content'], hash)
    
    
def generate_types_ipp(filename, tpl, types):
    hash = get_hash(tpl, types)
    if check_file(filename, hash):
    	return
    
    filecontent = ''
    for type in types:
        filecontent += tpl.replace("{type}", type) + "\n"
    
    save_file(filename, filecontent, hash)