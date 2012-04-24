data-structures
===============

rollback
========

compilation
-----------

    make rollback

testing
-------

    for i in tests/{small,large}
    do
        diff "$i".a <(bin/rollback < "$i") && echo "$i: ok" || echo "$i: failed"
    done