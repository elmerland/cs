      implicit none
      include 'mpif.h'
      integer myid, numprocs, ierr

      call mpi_init( ierr )
      call mpi_comm_rank( mpi_comm_world, myid, ierr )
      call mpi_comm_size( mpi_comm_world, numprocs, ierr )

      print *, "hello from ", myid, " of ", numprocs

      call mpi_finalize( ierr )
      stop
      end
