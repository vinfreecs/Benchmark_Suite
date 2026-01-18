// 1. Compute r0 := b − Ax0, z0 = M−1
// r0, and p0 := z0
// 2. For j = 0, 1, . . ., until convergence Do:
// 3. αj := (rj , zj )/(Apj , pj )
// 4. xj+1 := xj + αjpj
// 5. rj+1 := rj − αjApj
// 6. zj+1 := M−1*rj+1
// 7. βj := (rj+1, zj+1)/(rj , zj )
// 8. pj+1 := zj+1 + βjpj
// 9. EndDo