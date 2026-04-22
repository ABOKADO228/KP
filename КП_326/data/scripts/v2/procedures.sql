ROLLBACK;

/*==============================================================================
Назначить сотрудника на участок
================================================================================*/
CREATE OR REPLACE PROCEDURE pr_assign_employee_to_plot(
    IN p_farm_employee_id integer,
    IN p_farm_plot_id integer,
    IN p_assignment_type varchar(9),
    IN p_assigned_at date,
    IN p_notes text
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM public.employee_plot_assignment
        WHERE farm_employee_id = p_farm_employee_id
          AND farm_plot_id = p_farm_plot_id
          AND unassigned_at IS NULL
    ) THEN
        RAISE EXCEPTION 'Сотрудник уже назначен на этот участок';
    END IF;

    INSERT INTO public.employee_plot_assignment (
        farm_employee_id,
        farm_plot_id,
        assignment_type,
        assigned_at,
        unassigned_at,
        notes
    )
    VALUES (
        p_farm_employee_id,
        p_farm_plot_id,
        p_assignment_type,
        p_assigned_at,
        NULL,
        p_notes
    );
END;
$$;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --
BEGIN;

CALL pr_assign_employee_to_plot(
    1,
    1,
    'temporary',
    DATE '2026-04-22',
    'Демо'
);

SELECT *
FROM public.employee_plot_assignment
WHERE farm_employee_id = 1
  AND farm_plot_id = 1
ORDER BY id DESC;

ROLLBACK;

/*==============================================================================
Снять сотрудника с участка
================================================================================*/
CREATE OR REPLACE PROCEDURE pr_unassign_employee_from_plot(
    IN p_farm_employee_id integer,
    IN p_farm_plot_id integer,
    IN p_unassigned_at date
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM public.employee_plot_assignment
        WHERE farm_employee_id = p_farm_employee_id
          AND farm_plot_id = p_farm_plot_id
          AND unassigned_at IS NULL
    ) THEN
        RAISE EXCEPTION 'Активное назначение сотрудника на этот участок не найдено';
    END IF;

    UPDATE public.employee_plot_assignment
    SET unassigned_at = p_unassigned_at
    WHERE farm_employee_id = p_farm_employee_id
      AND farm_plot_id = p_farm_plot_id
      AND unassigned_at IS NULL;
END;
$$;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --
BEGIN;

CALL pr_unassign_employee_from_plot(
    1,
    128,
    DATE '2026-04-22'
);

SELECT *
FROM public.employee_plot_assignment
WHERE farm_employee_id = 1
  AND farm_plot_id = 128
ORDER BY id DESC;

ROLLBACK;

/*==============================================================================
Перевести сотрудника с одного участка на другой
================================================================================*/
CREATE OR REPLACE PROCEDURE pr_transfer_employee_to_another_plot(
    IN p_farm_employee_id integer,
    IN p_old_farm_plot_id integer,
    IN p_new_farm_plot_id integer,
    IN p_assignment_type varchar(9),
    IN p_transfer_date date,
    IN p_notes text
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM public.employee_plot_assignment
        WHERE farm_employee_id = p_farm_employee_id
          AND farm_plot_id = p_old_farm_plot_id
          AND unassigned_at IS NULL
    ) THEN
        RAISE EXCEPTION 'Сотрудник не назначен на исходный участок';
    END IF;

    IF EXISTS (
        SELECT 1
        FROM public.employee_plot_assignment
        WHERE farm_employee_id = p_farm_employee_id
          AND farm_plot_id = p_new_farm_plot_id
          AND unassigned_at IS NULL
    ) THEN
        RAISE EXCEPTION 'Сотрудник уже назначен на новый участок';
    END IF;

    UPDATE public.employee_plot_assignment
    SET unassigned_at = p_transfer_date
    WHERE farm_employee_id = p_farm_employee_id
      AND farm_plot_id = p_old_farm_plot_id
      AND unassigned_at IS NULL;

    INSERT INTO public.employee_plot_assignment (
        farm_employee_id,
        farm_plot_id,
        assignment_type,
        assigned_at,
        unassigned_at,
        notes
    )
    VALUES (
        p_farm_employee_id,
        p_new_farm_plot_id,
        p_assignment_type,
        p_transfer_date,
        NULL,
        p_notes
    );
END;
$$;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

SELECT farm_plot_id
FROM public.employee_plot_assignment
WHERE farm_employee_id = 1
  AND unassigned_at IS NULL;


BEGIN;

CALL pr_transfer_employee_to_another_plot(
    1,
    188,
    2,
    'primary',
    DATE '2026-04-22',
    'Демо'
);

SELECT *
FROM public.employee_plot_assignment
WHERE farm_employee_id = 1
  AND farm_plot_id IN (188, 2)
ORDER BY id DESC, farm_plot_id;

ROLLBACK;


/*==============================================================================
Принять сотрудника на ферму
================================================================================*/
CREATE OR REPLACE PROCEDURE pr_hire_farm_employee(
    IN p_person_id integer,
    IN p_farm_id integer,
    IN p_role_id integer,
    IN p_employment_status_id integer,
    IN p_hire_date date,
    IN p_salary numeric(12,2),
    IN p_employment_contract_number varchar(50),
    IN p_is_primary_workplace integer
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF p_salary < 0 THEN
        RAISE EXCEPTION 'Зарплата не может быть отрицательной';
    END IF;

    IF EXISTS (
        SELECT 1
        FROM public.farm_employee
        WHERE person_id = p_person_id
          AND farm_id = p_farm_id
          AND dismissal_date IS NULL
    ) THEN
        RAISE EXCEPTION 'Этот человек уже числится активным сотрудником данной фермы';
    END IF;

    INSERT INTO public.farm_employee (
        person_id,
        farm_id,
        role_id,
        employment_status_id,
        hire_date,
        dismissal_date,
        salary,
        employment_contract_number,
        is_primary_workplace
    )
    VALUES (
        p_person_id,
        p_farm_id,
        p_role_id,
        p_employment_status_id,
        p_hire_date,
        NULL,
        p_salary,
        p_employment_contract_number,
        p_is_primary_workplace
    );
END;
$$;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

CALL pr_hire_farm_employee(
    1, 
    1, 
    2,
    1,
    DATE '2026-04-22',
    65000.00,
    'DEMO-FE-2026-001',
    1 
);

SELECT *
FROM public.farm_employee
WHERE person_id = 1
  AND farm_id = 1
ORDER BY id DESC;

ROLLBACK;

/*==============================================================================
Добавить позицию в заказ закупки
================================================================================*/
CREATE OR REPLACE PROCEDURE pr_add_purchase_order_item(
    IN p_purchase_order_id integer,
    IN p_product_id integer,
    IN p_quantity numeric(12,3),
    IN p_unit_price numeric(12,2),
    IN p_vat_rate numeric(5,2),
    IN p_currency varchar(3)
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF p_quantity <= 0 THEN
        RAISE EXCEPTION 'Количество должно быть больше нуля';
    END IF;

    IF p_unit_price < 0 THEN
        RAISE EXCEPTION 'Цена не может быть отрицательной';
    END IF;

    INSERT INTO public.purchase_order_item (
        purchase_order_id,
        product_id,
        quantity,
        unit_price,
        vat_rate,
        currency
    )
    VALUES (
        p_purchase_order_id,
        p_product_id,
        p_quantity,
        p_unit_price,
        p_vat_rate,
        p_currency
    );
END;
$$;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

CALL pr_add_purchase_order_item(
    2,
    1,
    15.500,
    420.00,
    20.00,
    'RUB'
);

SELECT *
FROM public.purchase_order_item
WHERE purchase_order_id = 2
ORDER BY id DESC
LIMIT 5;

ROLLBACK;


/*==============================================================================
Удалить позицию из заказа закупки
================================================================================*/
CREATE OR REPLACE PROCEDURE pr_delete_purchase_order_item(
    IN p_purchase_order_item_id integer
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM public.purchase_order_item
        WHERE id = p_purchase_order_item_id
    ) THEN
        RAISE EXCEPTION 'Позиция заказа не найдена';
    END IF;

    DELETE FROM public.purchase_order_item
    WHERE id = p_purchase_order_item_id;
END;
$$;


-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --
BEGIN;

CALL pr_add_purchase_order_item(
    2,
    1,
    10.000,
    300.00,
    20.00,
    'RUB'
);

CALL pr_delete_purchase_order_item(890);

SELECT  *
FROM public.purchase_order_item
WHERE purchase_order_id = 2
ORDER BY id DESC
LIMIT 5;

ROLLBACK;