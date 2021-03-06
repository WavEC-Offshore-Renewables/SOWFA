    // Solve the momentum equation

    #include "computeCoriolisForce.H"
    #include "computeSpongeForce.H"

    #include "computeBuoyancyTerm.H"

    fvVectorMatrix UEqn
    (
        fvm::ddt(U)                             // time derivative
      + fvm::div(phi, U)                        // convection
      + turbulence->divDevReff(U)               // momentum flux
      + fvc::div(Rwall)
      - fCoriolis                               // Coriolis force
      - fSponge                                 // Sponge layer damping
      - SourceU                                 // mesoscale source terms
    );

    UEqn.relax();

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
            ==
            fvc::reconstruct
            (
                (
                  - fvc::snGrad(p_rgh)          // modified pressure gradient
                  + buoyancyTerm                // buoyancy force
                ) * mesh.magSf()
            )
        );
    }
