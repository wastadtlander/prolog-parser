import pexpect
import pexpect.replwrap as replwrap
import unittest
import tempfile
import os

# the vtpl executable
cmd = './vtpl'

# the prompt to expect
prompt = u'vtpl> '

class TestMissingArgument(unittest.TestCase):
    
    def test_error(self):
        (output, retcode) = pexpect.run(cmd, withexitstatus=True)
        self.assertNotEqual(retcode, 0)
        self.assertTrue(output.strip().startswith(b'Error'))
        
class TestNonExistant(unittest.TestCase):
    
    def test_error(self):
        args = ' /there/is/no/such/file'
        (output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
        self.assertNotEqual(retcode, 0)
        self.assertTrue(output.strip().startswith(b'Error'))

class TestBadFile(unittest.TestCase):

    def setUp(self):
        self.tempdir = tempfile.TemporaryDirectory()
        self.filename = os.path.join(self.tempdir.name,"badfile.pro") 
        # write the file
        with open(self.filename, 'w') as file:
            file.write("f(a).\nf(X) :-\n")
    
    def test_error(self):
        args = ' '+self.filename
        (output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
        self.assertNotEqual(retcode, 0)
        self.assertTrue(output.strip().startswith(b'Error'))

class Test00(unittest.TestCase):

    def setUp(self):

        content = """
friends(X,Y) :- likes(X,Z), likes(Y,Z).
likes(bill,movies).
likes(sally,movies).
likes(bob,pizza).
        """
        self.tempdir = tempfile.TemporaryDirectory()
        self.filename = os.path.join(self.tempdir.name,"likes.pro") 
        # write the file
        with open(self.filename, 'w') as file:
            file.write(content)

        args = ' '+self.filename
        self.wrapper = replwrap.REPLWrapper(cmd+args, prompt, None)

    def tearDown(self):
        self.tempdir.cleanup()
        
    def test_unknown_command(self):
        output = self.wrapper.run_command(u'foo')
        self.assertTrue(output.strip().startswith('Error'))

    def test_query_parse_error(self):
        output = self.wrapper.run_command(u'? likes(bill,movies)')
        self.assertTrue(output.strip().startswith('Parse Error'))
        
    def test_failed_resolution(self):
        output = self.wrapper.run_command(u'?f.')
        self.assertEqual(output.strip(), "false.")

    def test_successful_resolution(self):
        output = self.wrapper.run_command(u'? likes(sally,movies).')
        self.assertEqual(output.strip(), "(likes(sally,movies))")
        output = self.wrapper.run_command(u'? likes(X,movies).')
        lines = output.split()
        self.assertTrue((lines[0] == "(likes(bill,movies))") or (lines[1] == "(likes(bill,movies))"))
        self.assertTrue((lines[0] == "(likes(sally,movies))") or (lines[1] == "(likes(sally,movies))"))
        self.assertNotEqual(lines[0], lines[1])

        results = {"(friends(bill,bill))",
                   "(friends(sally,bill))", 
                   "(friends(bill,sally))",
                   "(friends(sally,sally))",
                   "(friends(bob,bob))"}

        output = self.wrapper.run_command(u'? friends(X,Y).')
        lines = output.split()
        temp = results.intersection(lines)
        self.assertEqual(len(temp), 5)

class Test01(unittest.TestCase):

    def setUp(self):

        content = """
hangout(X,Y) :- friends(X,Y),intown(X),intown(Y).
friends(X,Y) :- likes(X,Z), likes(Y,Z).
likes(bill,movies).
likes(sally,movies).
likes(bob,pizza).
likes(joe, movies).
intown(bill).
intown(sally).
intown(bob).
        """
        self.tempdir = tempfile.TemporaryDirectory()
        self.filename = os.path.join(self.tempdir.name,"likes2.pro") 
        # write the file
        with open(self.filename, 'w') as file:
            file.write(content)

        args = ' '+self.filename
        self.wrapper = replwrap.REPLWrapper(cmd+args, prompt, None)

    def tearDown(self):
        self.tempdir.cleanup()
        
    def test_ask(self):

        output = self.wrapper.run_command(u'? hangout(X,Y).')
        results = {"(hangout(bill,bill))",
                   "(hangout(sally,bill))",
                   "(hangout(bill,sally))",
                   "(hangout(sally,sally))",
                   "(hangout(bob,bob))"}
        lines = output.split()
        temp = results.intersection(lines)
        self.assertEqual(len(temp), 5)
            
class Test02(unittest.TestCase):

    def setUp(self):

        content = """
female(pam).
female(liz).
female(pat).
female(ann).
male(jim).
male(bob).
male(tom).
male(peter).
parent(pam,bob).
parent(tom,bob).
parent(tom,liz).
parent(bob,ann).
parent(bob,pat).
parent(pat,jim).
parent(bob,peter).
parent(peter,jim).
mother(X,Y):- parent(X,Y),female(X).
father(X,Y):- parent(X,Y),male(X).
haschild(X):- parent(X,_).
sister(X,Y):- parent(Z,X),parent(Z,Y),female(X).
brother(X,Y):-parent(Z,X),parent(Z,Y),male(X).
        """
        self.tempdir = tempfile.TemporaryDirectory()
        self.filename = os.path.join(self.tempdir.name,"family.pro") 
        # write the file
        with open(self.filename, 'w') as file:
            file.write(content)

        args = ' '+self.filename
        self.wrapper = replwrap.REPLWrapper(cmd+args, prompt, None)

    def tearDown(self):
        self.tempdir.cleanup()
        
    def test_ask(self):
        output = self.wrapper.run_command(u'? female(liz).')
        self.assertEqual(output.strip(), "(female(liz))")

        output = self.wrapper.run_command(u'? parent(X,bob).')
        results = {"(parent(pam,bob))", "(parent(tom,bob))"}
        lines = output.split()
        temp = results.intersection(lines)
        self.assertEqual(len(temp), 2)

        output = self.wrapper.run_command(u'? haschild(X).')
        results = {"(haschild(pam))",
                   "(haschild(tom))",
                   "(haschild(bob))",
                   "(haschild(pat))",
                   "(haschild(peter))"}
        lines = output.split()
        temp = results.intersection(lines)
        self.assertEqual(len(temp), 5)
        
# run the tests
unittest.main()
