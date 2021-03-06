from clang.cindex import Index, File, SourceLocation, SourceRange, Cursor

baseInput="int one;\nint two;\n"

def assert_location(loc, line, column, offset):
    assert loc.line == line
    assert loc.column == column
    assert loc.offset == offset

def test_location():
    index = Index.create()
    tu = index.parse('t.c', unsaved_files = [('t.c',baseInput)])

    for n in tu.cursor.get_children():
        if n.spelling == 'one':
            assert_location(n.location,line=1,column=5,offset=4)
        if n.spelling == 'two':
            assert_location(n.location,line=2,column=5,offset=13)

    # adding a linebreak at top should keep columns same
    tu = index.parse('t.c', unsaved_files = [('t.c',"\n"+baseInput)])

    for n in tu.cursor.get_children():
        if n.spelling == 'one':
            assert_location(n.location,line=2,column=5,offset=5)
        if n.spelling == 'two':
            assert_location(n.location,line=3,column=5,offset=14)

    # adding a space should affect column on first line only
    tu = index.parse('t.c', unsaved_files = [('t.c'," "+baseInput)])

    for n in tu.cursor.get_children():
        if n.spelling == 'one':
            assert_location(n.location,line=1,column=6,offset=5)
        if n.spelling == 'two':
            assert_location(n.location,line=2,column=5,offset=14)

    # define the expected location ourselves and see if it matches
    # the returned location
    tu = index.parse('t.c', unsaved_files = [('t.c',baseInput)])

    file = File.from_name(tu, 't.c')
    location = SourceLocation.from_position(tu, file, 1, 5)
    cursor = Cursor.from_location(tu, location)

    for n in tu.cursor.get_children():
        if n.spelling == 'one':
            assert n == cursor

    # Ensure locations referring to the same entity are equivalent.
    location2 = SourceLocation.from_position(tu, file, 1, 5)
    assert location == location2
    location3 = SourceLocation.from_position(tu, file, 1, 4)
    assert location2 != location3

def test_extent():
    index = Index.create()
    tu = index.parse('t.c', unsaved_files = [('t.c',baseInput)])

    for n in tu.cursor.get_children():
        if n.spelling == 'one':
            assert_location(n.extent.start,line=1,column=1,offset=0)
            assert_location(n.extent.end,line=1,column=8,offset=7)
            assert baseInput[n.extent.start.offset:n.extent.end.offset] == "int one"
        if n.spelling == 'two':
            assert_location(n.extent.start,line=2,column=1,offset=9)
            assert_location(n.extent.end,line=2,column=8,offset=16)
            assert baseInput[n.extent.start.offset:n.extent.end.offset] == "int two"

    file = File.from_name(tu, 't.c')
    location1 = SourceLocation.from_position(tu, file, 1, 1)
    location2 = SourceLocation.from_position(tu, file, 1, 8)

    range1 = SourceRange.from_locations(location1, location2)
    range2 = SourceRange.from_locations(location1, location2)
    assert range1 == range2

    location3 = SourceLocation.from_position(tu, file, 1, 6)
    range3 = SourceRange.from_locations(location1, location3)
    assert range1 != range3
